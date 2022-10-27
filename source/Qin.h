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

template<class T, class Fn = nothing<T>>
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

template<class T, class Fn>
class Qin : public QinBase {
    T  value;
    Fn effect;

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
        if (!std::is_same<decltype(effect), nothing<T>>::value) {
            return effect(Heng[0]->template into<T>().get(), Heng[1]->template into<T>().get());
        }
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

    operator T() {
        return get();
    }

    template<class FnNew>
    void setEff(FnNew eff) {
        this->effect = eff;
    }

    template<class FnNew>
    Qin<T, FnNew>& lian(QinBase* q, FnNew eff) {
        auto new_qin = new Qin<T, FnNew> { T {} };
        new_qin->QinBase::lian(this, q);
        new_qin->setEff(eff);
        return *new_qin;
    }

    template<class FnNew>
    Qin<T, FnNew>& lian(QinBase* q) {
        auto new_qin = new Qin<T, FnNew> { T {} };
        new_qin->QinBase::lian(this, q);
        new_qin->setEff(FnNew());
        return *new_qin;
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
