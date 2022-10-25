//
// Created by W-Mai on 2022/10/22.
//

#include "Qin.h"

void operator<<(QinBase& l, QinBase& r) {
    l.bind(&r);
}

void operator<<(QinBase& l, QinBase* r) {
    l.bind(r);
}

void operator<<(QinBase* l, QinBase& r) {
    l->bind(&r);
}

void QinBase::bind(QinBase* src) {
    auto& src_zong = src->Zong;
    if (std::find(src_zong.begin(), src_zong.end(), this) != src_zong.end()) {
        return;
    }

    src_zong.push_back(this);
}
