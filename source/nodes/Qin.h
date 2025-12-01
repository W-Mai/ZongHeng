//
// Qin - Homogeneous node (Qin<T> = Yi<T, T>)
//

#ifndef ZONGHENG_NODES_QIN_H
#define ZONGHENG_NODES_QIN_H

#include "Yi.h"

// ============================================================================
// Qin - Homogeneous Node Template
// ============================================================================

template<class T>
class Qin : public Yi<T, T> {
public:
    using SharedQin_T = std::shared_ptr<Qin<T>>;

    using Yi<T, T>::operator=;

    // Arithmetic operators
    friend SharedQin_T operator+(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::plus<T>>(q);
    }

    friend SharedQin_T operator-(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::minus<T>>(q);
    }

    friend SharedQin_T operator*(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::multiplies<T>>(q);
    }

    friend SharedQin_T operator/(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::divides<T>>(q);
    }

    friend SharedQin_T operator%(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::modulus<T>>(q);
    }

    // Bitwise operators
    friend SharedQin_T operator&(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::bit_and<T>>(q);
    }

    friend SharedQin_T operator|(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::bit_or<T>>(q);
    }

    friend SharedQin_T operator^(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::bit_xor<T>>(q);
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

    // ========================================================================
    // Functional Combinators (Chainable)
    // ========================================================================

    /**
     * @brief Map this node's value through a transformation function
     * @param fn Transformation function (T -> OUT)
     * @return New Qin node with transformed value
     * @example auto doubled = nums->map([](int x) { return x * 2; });
     */
    template<class Fn>
    auto map(Fn fn) -> std::shared_ptr<Qin<decltype(fn(std::declval<T>()))>> {
        using OUT = decltype(fn(std::declval<T>()));
        auto result = Qin<OUT>::make(OUT{});
        result->QinBase::lian(this->self, this->self);
        result->setEff([self = this->self, fn]() -> OUT {
            return fn(self->template into<T, T>()->get());
        });
        return result;
    }

    /**
     * @brief Filter this node's value based on a predicate
     * @param defaultVal Default value when predicate fails
     * @param predicate Boolean function (T -> bool)
     * @return New Qin node with filtered value
     * @example auto positive = value->filter(fallback, [](int x) { return x > 0; });
     */
    template<class Pred>
    SharedQin_T filter(SharedQin_T defaultVal, Pred predicate) {
        auto result = Qin<T>::make(T{});
        result->QinBase::lian(this->self, defaultVal);
        result->setEff([self = this->self, defaultVal, predicate]() -> T {
            return predicate(self->template into<T, T>()->get())
                ? self->template into<T, T>()->get()
                : defaultVal->get();
        });
        return result;
    }

    /**
     * @brief Conditionally select between this value and another
     * @param condition Boolean Qin node
     * @param falseVal Value to use when condition is false
     * @return New Qin node with selected value
     * @example auto selected = optionA->when(flag, optionB);
     */
    SharedQin_T when(std::shared_ptr<Qin<bool>> condition, SharedQin_T falseVal) {
        auto result = Qin<T>::make(T{});
        result->QinBase::lian(condition, condition);
        this->addDerivedNode(result);
        falseVal->addDerivedNode(result);

        result->setEff([self = this->self, condition, falseVal]() -> T {
            return condition->get()
                ? self->template into<T, T>()->get()
                : falseVal->get();
        });
        return result;
    }

    // Factory method
    template<class... ARGS>
    static SharedQin_T make(ARGS&&... val) {
        auto ptr  = std::static_pointer_cast<Qin<T>>(Yi<T, T>::make(std::forward<ARGS>(val)...));
        ptr->self = ptr;
        return ptr;
    }
};

#endif // ZONGHENG_NODES_QIN_H
