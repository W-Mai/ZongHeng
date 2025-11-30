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

    // Operator overloads for arithmetic composition
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

    // Factory method
    template<class... ARGS>
    static SharedQin_T make(ARGS&&... val) {
        auto ptr  = std::static_pointer_cast<Qin<T>>(Yi<T, T>::make(std::forward<ARGS>(val)...));
        ptr->self = ptr;
        return ptr;
    }
};

#endif // ZONGHENG_NODES_QIN_H
