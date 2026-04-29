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

struct CalculationRequest {
    long long left{};
    std::optional<long long> right{};
    Operation operation{};
};

struct ParseResult {
    bool help_requested{false};
    std::optional<CalculationRequest> request{};
};

}