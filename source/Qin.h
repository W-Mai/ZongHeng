//
// Created by W-Mai on 2022/10/22.
//

#ifndef ZONGHENG_QIN_H
#define ZONGHENG_QIN_H

class QinBase {
public:
    virtual void print() {};
};

template<class T>
class Qin : public QinBase {
private:
    T value;

public:
    explicit Qin(T v)
        : value(v) { }

    void print() override;
};

#include <iostream>

template<class T>
void Qin<T>::print() {
    std::cout << value << std::endl;
}

#endif // ZONGHENG_QIN_H
