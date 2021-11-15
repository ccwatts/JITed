#pragma once

#include <memory>
#include <string>
#include <typeinfo>

namespace jited {

template <typename T>
const char* typeOf(std::shared_ptr<T> p) {
    return typeid(*p).name();
}

template <typename T>
std::string commonType(std::shared_ptr<T> a, std::shared_ptr<T> b) {
    // return typeid(*a).name() == typeid(*b).name();
    std::string typeA = typeOf<T>(a), typeB = typeOf<T>(b);
    if (typeA == typeB) {
        return typeA;
    } else {
        return "";
    }
}

}