//
// Created by W-Mai on 2022/10/22.
//

#ifndef ZONGHENG_QIN_H
#define ZONGHENG_QIN_H

#include <vector>

template<class T>
class Qin;

class QinBase {
protected:
    using Zong_t = std::vector<QinBase*>;
    //    using Heng = std::vector<QinBase*>;
    Zong_t Zong;

public:
    friend void operator<<(QinBase& l, QinBase& r);
    friend void operator<<(QinBase& l, QinBase* r);
    friend void operator<<(QinBase* l, QinBase& r);

    void bind(QinBase* src);

    template<class T>
    Qin<T>& into() {
        return *reinterpret_cast<Qin<T>*>(this);
    }
};

template<class T>
class Qin : public QinBase {
    T value;

public:
    Qin(Qin<T>& v) noexcept
        : value(std::forward<T>(v.value)) { }

    explicit Qin(T& v)
        : value(std::forward<T>(v)) { }

    explicit Qin(T&& v)
        : value(std::forward<T>(v)) { }

    void set(T&& val) {
        value = std::forward<T>(val);
        for (QinBase* qin : Zong) {
            qin->into<T>().set(std::forward<T>(val));
        }
    }

    T get() {
        return value;
    }

    Qin<T>& operator=(T& val) {
        set(std::forward<T>(val));
        return *this;
    }

    Qin<T>& operator=(T&& val) {
        set(std::forward<T>(val));
        return *this;
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
