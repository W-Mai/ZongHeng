//
// Created by W-Mai on 2022/10/26.
//

#ifndef EXAMPLE_QINUTILS_H
#define EXAMPLE_QINUTILS_H

template<typename T>
Qin<T>& as(QinBase* qb) {
    return qb->into<T>();
}

#endif // EXAMPLE_QINUTILS_H
