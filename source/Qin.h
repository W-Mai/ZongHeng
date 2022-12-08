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

    template<class T>
    typename Qin<T>::SharedQin_T into() {
        return std::static_pointer_cast<Qin<T>>(self);
    }

    void lian(std::shared_ptr<QinBase> q1, std::shared_ptr<QinBase> q2) {
        Heng.push_back(q1);
        Heng.push_back(q2);
    }
};

template<class T>
class Qin : public QinBase {
public:
    using SharedQin_T = std::shared_ptr<Qin<T>>;

protected:
    T                          rawValue; // 不知道怎么才能合理引用🤔，暂时先复制吧
    T                          getterValue; // 不知道怎么才能合理引用🤔，暂时先复制吧
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
        value    = [=]() -> const T& {
            return rawValue;
        };
        for (auto& qin : Zong) {
            qin->template into<T>()->set(std::forward<T>(val));
        }
    }

    const T& get() {
        T v;
        if (effect) {
            v = effect();
        } else {
            v = value();
        }
        if (_getter) {
            getterValue = _getter(v);
            return getterValue;
        }

        rawValue = v;
        return rawValue;
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
    SharedQin_T lian(std::shared_ptr<QinBase> q, Fn eff) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(self, q);
        new_qin->setEff(eff);
        return new_qin;
    }

    template<class Fn>
    SharedQin_T lian(std::shared_ptr<QinBase> q) {
        auto new_qin = Qin<T>::make(T {});
        new_qin->QinBase::lian(self, q);
        new_qin->setEff([this, q]() -> T {
            return Fn()(this->get(), q->template into<T>()->get());
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

    friend SharedQin_T operator+(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::plus<T>>(q);
    }

    friend SharedQin_T operator*(SharedQin_T p, SharedQin_T q) {
        return p->template lian<std::multiplies<T>>(&q);
    }

    // Utils
public:
    template<class... ARGS>
    static SharedQin_T make(ARGS&&... val) {
        auto ptr  = std::make_shared<Qin<T>>(val...);
        ptr->self = ptr;
        return ptr;
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
