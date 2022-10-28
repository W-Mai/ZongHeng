//
// Created by W-Mai on 2022/10/22.
//

#ifndef ZONGHENG_QIN_H
#define ZONGHENG_QIN_H

#include <functional>
#include <vector>

template<class T>
struct nothing : std::binary_function<T, T, T> {
    typedef T result_type;
    typedef T first_argument_type;
    typedef T second_argument_type;

    T operator()(const T& x, const T& y) const { return 0; }
};

template<class T>
class Qin;

class QinBase {
protected:
    using Zong_t = std::vector<QinBase*>;
    using Heng_t = std::vector<QinBase*>;

    Zong_t Zong;
    Heng_t Heng;

public:
    friend void operator<<(QinBase& l, QinBase& r);
    friend void operator<<(QinBase& l, QinBase* r);
    friend void operator<<(QinBase* l, QinBase& r);

    void bind(QinBase* src);

    template<class T>
    Qin<T>& into() {
        return *reinterpret_cast<Qin<T>*>(this);
    }

    void lian(QinBase* q1, QinBase* q2) {
        Heng.push_back(q1);
        Heng.push_back(q2);
    }
};

template<class T>
class Qin : public QinBase {
    std::function<T()> value;
    std::function<T()> effect;

public:
    Qin(Qin<T>& v) noexcept {
        set(std::forward<T>(v));
    }

    explicit Qin(T& v) {
        set(std::forward<T>(v));
    }

    explicit Qin(T&& v) {
        set(std::forward<T>(v));
    }

    void set(T&& val) {
        value = [&]() -> T {
            return std::forward<T>(val);
        };
        for (QinBase* qin : Zong) {
            qin->into<T>().set(std::forward<T>(val));
        }
    }

    T get() {
        if (effect) {
            return effect();
        }
        return value();
    }

    Qin<T>& operator=(T& val) {
        set(std::forward<T>(val));
        return *this;
    }

    Qin<T>& operator=(T&& val) {
        set(std::forward<T>(val));
        return *this;
    }

    operator T() {
        return get();
    }

    template<class Fn>
    void setEff(Fn eff) {
        this->effect = eff;
    }

    template<class Fn>
    Qin<T>& lian(QinBase* q, Fn eff) {
        auto new_qin = new Qin<T> { T {} };
        new_qin->QinBase::lian(this, q);
        new_qin->setEff(eff);
        return *new_qin;
    }

    template<class Fn>
    Qin<T>& lian(QinBase* q) {
        auto new_qin = new Qin<T> { T {} };
        new_qin->QinBase::lian(this, q);
        new_qin->setEff([this, q]() -> T {
            return Fn()(this->get(), q->template into<T>().get());
        });
        return *new_qin;
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
