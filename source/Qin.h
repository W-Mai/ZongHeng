//
// Created by W-Mai on 2022/10/22.
//

#ifndef ZONGHENG_QIN_H
#define ZONGHENG_QIN_H

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

template<class INPUT_TYPE, class OUTPUT_TYPE>
class Yi;

#define FORWARD_CONSTRAINT(TYPE, TYPE_INNER) <class TYPE, \
    class = std::enable_if<                                                \
        std::is_same<std::remove_cv<TYPE>, TYPE_INNER>::value              \
        || std::is_constructible<TYPE, TYPE_INNER>::value                  \
        || std::is_convertible<TYPE, TYPE_INNER>::value>>

template<class INPUT, class OUTPUT>
typename std::enable_if<std::is_same<typename std::is_convertible<INPUT, OUTPUT>::type, std::true_type>::value, OUTPUT>::type
convert(const INPUT& input) {
    return static_cast<OUTPUT>(input);
}

template<class INPUT, class OUTPUT>
typename std::enable_if<std::is_same<typename std::is_convertible<INPUT, OUTPUT>::type, std::false_type>::value, OUTPUT>::type
convert(const INPUT&) {
    return OUTPUT {};
}

class QinBase {
public:
    using SharedQinBase_T = std::shared_ptr<QinBase>;

protected:

    using Zong_t = std::vector<SharedQinBase_T>;
    using Heng_t = std::vector<SharedQinBase_T>;

    Zong_t Zong;
    Heng_t Heng;

    SharedQinBase_T self;

public:
    friend void operator<<(std::shared_ptr<QinBase> l, std::shared_ptr<QinBase> r);

    void bind(const std::shared_ptr<QinBase>& src);

    template<class IN, class OUT>
    typename Yi<IN, OUT>::SharedYi_T into() {
        return std::static_pointer_cast<Yi<IN, OUT>>(self);
    }

    void lian(const SharedQinBase_T& q1, const SharedQinBase_T& q2) {
        q1->Heng.push_back(self);
        q2->Heng.push_back(self);
    }
};

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
    std::function<NoneCVTOutput()>                    effect;
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

        // Update
        for (auto& yi : Zong) {
            yi->template into<INPUT_TYPE, OUTPUT_TYPE>()->set(std::forward<NoneCVTOutput>(tmp_val));
        }

        for (auto& heng : Heng) {
            auto& h_ef = heng->template into<INPUT_TYPE, OUTPUT_TYPE>()->effect;
            if (h_ef)
                heng->template into<INPUT_TYPE, OUTPUT_TYPE>()->set(h_ef());
        }
    }

    NoneCVTOutput get() {
        if (effect) {
            return effect();
        }

        const NoneCVTInput v = value();
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

    // Utils
public:
    template<class... ARGS>
    static SharedYi_T make(ARGS&&... val) {
        auto ptr  = std::make_shared<Yi<INPUT_TYPE, OUTPUT_TYPE>>(std::forward<ARGS>(val)...);
        ptr->self = ptr;
        return ptr;
    }
};

template<class T>
class Qin : public Yi<T, T> {
public:
    using SharedQin_T = std::shared_ptr<Qin<T>>;

    using Yi<T, T>::operator=;

    friend SharedQin_T operator+(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::plus<T>>(q);
    }

    friend SharedQin_T operator*(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::multiplies<T>>(q);
    }

    template<class Fn>
    SharedQin_T lian(std::shared_ptr<QinBase> q, Fn eff) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(this->self, q);
        new_qin->setEff(eff);
        return new_qin;
    }

    template<class Fn>
    SharedQin_T lian(std::shared_ptr<QinBase> q) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(this->self, q);
        new_qin->setEff([this, q]() -> T {
            return Fn()(this->get(), q->template into<T, T>()->get());
        });
        return new_qin;
    }

    template<class... ARGS>
    static SharedQin_T make(ARGS&&... val) {
        auto ptr  = std::static_pointer_cast<Qin<T>>(Yi<T, T>::make(std::forward<ARGS>(val)...));
        ptr->self = ptr;
        return ptr;
    }
};

#include "QinUtils.h"

#undef FORWARD_CONSTRAINT

#endif // ZONGHENG_QIN_H
