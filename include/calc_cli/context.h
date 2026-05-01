#pragma once

#include <safe_math/safe_math.hpp>

#include <optional>

namespace calc_cli {

enum class Operation {
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Factorial
};

struct ApplicationContext {
    long long left;
    long long right;
    Operation operation;
    long long result;
    int help_requested;
};

}