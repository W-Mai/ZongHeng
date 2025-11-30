//
// Extended operators for ZongHeng nodes
//

#ifndef ZONGHENG_OPERATIONS_OPERATORS_H
#define ZONGHENG_OPERATIONS_OPERATORS_H

#include "../nodes/Qin.h"

// ============================================================================
// Comparison Operators (return Qin<bool>)
// ============================================================================

template<class T>
std::shared_ptr<Qin<bool>> operator==(std::shared_ptr<Qin<T>> p,
                                       std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() == q->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator!=(std::shared_ptr<Qin<T>> p,
                                       std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() != q->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator<(std::shared_ptr<Qin<T>> p,
                                      std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() < q->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator>(std::shared_ptr<Qin<T>> p,
                                      std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() > q->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator<=(std::shared_ptr<Qin<T>> p,
                                       std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() <= q->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator>=(std::shared_ptr<Qin<T>> p,
                                       std::shared_ptr<Qin<T>> q) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, q);
    result->setEff([p, q]() -> bool {
        return p->get() >= q->get();
    });
    return result;
}

// ============================================================================
// Unary Operators
// ============================================================================

template<class T>
std::shared_ptr<Qin<T>> operator-(std::shared_ptr<Qin<T>> p) {
    auto result = Qin<T>::make(T{});
    result->QinBase::lian(p, p);
    result->setEff([p]() -> T {
        return -p->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<T>> operator~(std::shared_ptr<Qin<T>> p) {
    auto result = Qin<T>::make(T{});
    result->QinBase::lian(p, p);
    result->setEff([p]() -> T {
        return ~p->get();
    });
    return result;
}

template<class T>
std::shared_ptr<Qin<bool>> operator!(std::shared_ptr<Qin<T>> p) {
    auto result = Qin<bool>::make(false);
    result->QinBase::lian(p, p);
    result->setEff([p]() -> bool {
        return !static_cast<bool>(p->get());
    });
    return result;
}

#endif // ZONGHENG_OPERATIONS_OPERATORS_H
