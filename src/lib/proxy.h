#pragma once

#include <memory>
#include "antlr4-runtime.h"

namespace jited {

// necessary to be able to upcast Any values containing derived classes
template <typename U, typename V>
antlrcpp::Any proxy(std::shared_ptr<U> from) {
    auto proxied = std::static_pointer_cast<V>(from);
    return antlrcpp::Any(proxied);
}

}