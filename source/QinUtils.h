//
// Created by W-Mai on 2022/10/26.
//

#ifndef EXAMPLE_QINUTILS_H
#define EXAMPLE_QINUTILS_H

template<typename T, typename Q = QinBase::SharedQinBase_T>
typename Qin<T>::SharedQin_T as(Q&& qb) {
    return qb->template into<T>();
}

template<typename T, typename Q = QinBase::SharedQinBase_T>
typename Yi<T, T>::SharedYi_T as(Q&& qb) {
    return qb->template into<T, T>();
}

#endif // EXAMPLE_QINUTILS_H
