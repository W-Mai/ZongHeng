//
// Yi - Heterogeneous transformation node (INPUT_TYPE -> OUTPUT_TYPE)
//

#ifndef ZONGHENG_NODES_YI_H
#define ZONGHENG_NODES_YI_H

#include "../core/ZongHengBase.h"

// ============================================================================
// Yi - Heterogeneous Node Template
// ============================================================================

template<class INPUT_TYPE, class OUTPUT_TYPE>
class Yi : public QinBase {
public:
    using SharedYi_T = std::shared_ptr<Yi<INPUT_TYPE, OUTPUT_TYPE>>;

protected:
    using NoneCVTInput  = typename std::remove_cv<INPUT_TYPE>::type;
    using NoneCVTOutput = typename std::remove_cv<OUTPUT_TYPE>::type;

    NoneCVTInput                                      rawValue;
    NoneCVTOutput                                     getterValue;
    std::function<NoneCVTInput()>                     value;
    std::function<NoneCVTInput()>                     effect;
    std::function<NoneCVTInput(const NoneCVTOutput&)> _setter;
    std::function<NoneCVTOutput(const NoneCVTInput&)> _getter;

public:
    Yi() noexcept {
        set(NoneCVTInput {});
    }

    template FORWARD_CONSTRAINT(V, NoneCVTInput) explicit Yi(V&& v)
        : rawValue(v) {
        set_raw(std::forward<V>(v));
    }

    template FORWARD_CONSTRAINT(V, NoneCVTInput) void set_raw(V&& val) {
        rawValue = val;
        value    = [=]() -> const NoneCVTInput& {
            return rawValue;
        };
    }

    template FORWARD_CONSTRAINT(V, NoneCVTInput) void set_inner(V&& val) {
        rawValue = val;
    }

    template FORWARD_CONSTRAINT(V, NoneCVTOutput) void set(V&& val) {
        NoneCVTInput  tmp_out;
        NoneCVTOutput tmp_val { static_cast<NoneCVTOutput>(val) };

        if (_setter) {
            tmp_out = _setter(tmp_val);
        } else {
            tmp_out = convert<NoneCVTOutput, NoneCVTInput>(std::move(tmp_val));
        }

        set_raw(std::forward<NoneCVTInput>(tmp_out));

        // Update - only propagate to compatible types
        for (auto& yi : Zong) {
            try {
                yi->template into<INPUT_TYPE, OUTPUT_TYPE>()->set(std::forward<NoneCVTOutput>(tmp_val));
            } catch (const std::runtime_error&) {
                // Skip incompatible type nodes
            }
        }

        for (auto& heng : Heng) {
            try {
                const auto& h = heng->template into<INPUT_TYPE, OUTPUT_TYPE>();
                if (h->effect) {
                    auto h_ef_val = h->effect();
                    if (std::is_same_v<NoneCVTInput, NoneCVTOutput>) {
                        h->set(convert<NoneCVTInput, NoneCVTOutput>(h_ef_val));
                    } else if (h->_getter) {
                        auto h_out = h->_getter(h_ef_val);
                        h->set(h_out);
                    }
                }
            } catch (const std::runtime_error&) {
                // Skip incompatible type nodes
            }
        }
    }

    NoneCVTOutput get() {
        const auto v = effect ? effect() : value();

        if (_getter) {
            getterValue = _getter(v);
            return getterValue;
        }

        return convert<INPUT_TYPE, OUTPUT_TYPE>(v);
    }

    template FORWARD_CONSTRAINT(V, NoneCVTOutput) Yi<INPUT_TYPE, OUTPUT_TYPE>& operator=(V&& val) {
        set(std::forward<V>(val));
        return *this;
    }

    explicit operator NoneCVTOutput() {
        return get();
    }

    template<class Fn>
    void setEff(Fn eff) {
        this->effect = eff;
    }

    template<class Fn>
    SharedYi_T lian(std::shared_ptr<QinBase> q, Fn eff) {
        auto new_qin = Yi<INPUT_TYPE, OUTPUT_TYPE>::make(INPUT_TYPE {});
        new_qin->QinBase::lian(self, q);
        new_qin->setEff(eff);
        return new_qin;
    }

    template<class Fn>
    SharedYi_T lian(std::shared_ptr<QinBase> q) {
        auto new_qin = Yi<NoneCVTInput, NoneCVTOutput>::make(INPUT_TYPE {});
        new_qin->QinBase::lian(self, q);
        new_qin->setEff([this, q]() -> INPUT_TYPE {
            return Fn()(this->get(), q->template into<INPUT_TYPE, OUTPUT_TYPE>()->get());
        });
        return new_qin;
    }

    void setter(decltype(_setter) s) {
        _setter = s;
    }

    void getter(decltype(_getter) g) {
        _getter = g;
    }

    void hook(decltype(_getter) g = nullptr, decltype(_setter) s = nullptr) {
        getter(g);
        setter(s);
    }

    // Factory method
    template<class... ARGS>
    static SharedYi_T make(ARGS&&... val) {
        auto ptr  = std::make_shared<Yi<INPUT_TYPE, OUTPUT_TYPE>>(std::forward<ARGS>(val)...);
        ptr->self = ptr;
        return ptr;
    }
};

#endif // ZONGHENG_NODES_YI_H
