#pragma once

#include <safe_math/safe_math.hpp>

#include <cstdint>

namespace calc_cli {

enum class Operation : std::uint8_t {
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Factorial
};

struct ApplicationContext {
    long long left = 0;
    long long right = 0;
    Operation operation = Operation::Add;
    long long result = 0;
    int help_requested = 0;
    int parse_status = 0;
};

}