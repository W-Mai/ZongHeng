//
// Created by W-Mai on 2022/10/22.
//

#include "ZongHeng.h"

void operator<<(std::shared_ptr<QinBase> l, std::shared_ptr<QinBase> r) {
    l->bind(r);
}

void QinBase::bind(const std::shared_ptr<QinBase>& src) {
    auto& src_zong = src->Zong;
    if (std::find_if(src_zong.begin(), src_zong.end(), [this](const std::shared_ptr<QinBase>& t) {
            return t.get() == this;
        })
        != src_zong.end()) {
        return;
    }

    src_zong.push_back(self);
}
