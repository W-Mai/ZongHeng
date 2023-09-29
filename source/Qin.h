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

    void lian(SharedQinBase_T q1, SharedQinBase_T q2) {
        Heng.push_back(q1);
        Heng.push_back(q2);
    }
};

template<class T>
class Qin : public QinBase {
public:
    using SharedQin_T = std::shared_ptr<Qin<T>>;

protected:
    using NoneCVT = typename std::remove_cv<T>::type;

    NoneCVT                          rawValue;    // 不知道怎么才能合理引用🤔，暂时先复制吧
    NoneCVT                          getterValue; // 不知道怎么才能合理引用🤔，暂时先复制吧
    std::function<NoneCVT()>         value;
    std::function<NoneCVT()>         effect;
    std::function<NoneCVT(const T&)> _setter;
    std::function<NoneCVT(const T&)> _getter;

public:
    Qin() noexcept {
        set(NoneCVT {});
    }

    template<class V, class = std::enable_if<
                          std::is_same<std::remove_cv<V>, NoneCVT>::value
                          || std::is_constructible<V, NoneCVT>::value>>
    explicit Qin(V&& v)
        : rawValue(v) {
        set(std::forward<V>(v));
    }

    template<class V, class = std::enable_if<
                          std::is_same<std::remove_cv<V>, NoneCVT>::value
                          || std::is_constructible<V, NoneCVT>::value>>
    void set(V&& val) {
        NoneCVT tmp_val { val };

        if (_setter) {
            tmp_val = _setter(tmp_val);
        }
        rawValue = tmp_val;
        value    = [=]() -> const NoneCVT& {
            return rawValue;
        };
        // Update
        for (auto& qin : Zong) {
            qin->template into<T>()->set(std::forward<T>(tmp_val));
        }
    }

    const NoneCVT& get() {
        NoneCVT v;
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

    template<class V, class = std::enable_if<
                          std::is_same<std::remove_cv<V>, NoneCVT>::value
                          || std::is_constructible<V, NoneCVT>::value>>
    Qin<T>& operator=(V&& val) {
        set(std::forward<V>(val));
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
        auto new_qin = Qin<NoneCVT>::make(T {});
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
        auto ptr  = std::make_shared<Qin<T>>(std::forward<ARGS>(val)...);
        ptr->self = ptr;
        return ptr;
    }
};

#include "QinUtils.h"

#endif // ZONGHENG_QIN_H
