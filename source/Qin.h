//
// Created by W-Mai on 2022/10/22.
//

#ifndef ZONGHENG_QIN_H
#define ZONGHENG_QIN_H

#include <functional>
#include <memory>
#include <vector>

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
    using PTR_T = std::shared_ptr<Qin<T>>;

    T                          rawValue; // 不知道怎么才能合理引用🤔，暂时先复制吧
    std::function<T()>         value;
    std::function<T()>         effect;
    std::function<T(const T&)> _setter;
    std::function<T(const T&)> _getter;

public:
    Qin() noexcept {
        set(T {});
    }

    Qin(Qin<T>& v) noexcept {
        set(std::forward<T>(v));
    }

    explicit Qin(T& v)
        : rawValue(v) {
        set(std::forward<T>(v));
    }

    explicit Qin(T&& v)
        : rawValue(v) {
        set(std::forward<T>(v));
    }

    void set(T&& val) {
        if (_setter) {
            val = _setter(val);
        }
        rawValue = val;
        value    = [=]() -> T {
            return rawValue;
        };
        for (QinBase* qin : Zong) {
            qin->into<T>().set(std::forward<T>(val));
        }
    }

    T get() {
        T v;
        if (effect) {
            v = effect();
        } else {
            v = value();
        }
        if (_getter) {
            v = _getter(v);
        }
        return v;
    }

    Qin<T>& operator=(T& val) {
        set(std::forward<T>(val));
        return *this;
    }

    Qin<T>& operator=(T&& val) {
        set(std::forward<T>(val));
        return *this;
    }

    explicit operator T() {
        return get();
    }

    template<class Fn>
    void setEff(Fn eff) {
        this->effect = eff;
    }

    template<class Fn>
    Qin<T>& lian(std::shared_ptr<QinBase> q, Fn eff) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(this, q);
        new_qin->setEff(eff);
        return *new_qin;
    }

    template<class Fn>
    PTR_T lian(std::shared_ptr<QinBase> q) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(this, q);
        new_qin->setEff([this, q]() -> T {
            return Fn()(this->get(), q->template into<T>().get());
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

    friend PTR_T operator+(Qin<T>& p, Qin<T>& q) {
        return p.lian<std::plus<T>>(&q);
    }

    friend PTR_T operator+(PTR_T p, Qin<T>&& q) {
        return p->template lian<std::plus<T>>(&q);
    }

    friend PTR_T operator*(Qin<T>& p, Qin<T>& q) {
        return p.lian<std::multiplies<T>>(&q);
    }

    friend PTR_T operator*(PTR_T p, Qin<T>& q) {
        return p->template lian<std::multiplies<T>>(&q);
    }

    // Utils
public:
    template<class... ARGS>
    static PTR_T make(ARGS&&... val) {
        return std::make_shared<Qin<T>>(val...);
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
