#pragma once

#include <safe_math/safe_math.hpp>

namespace calc_cli {

    enum Operation {
        OPERATION_NONE = 0,
        OPERATION_ADD = 1,
        OPERATION_SUBTRACT = 2,
        OPERATION_MULTIPLY = 3,
        OPERATION_DIVIDE = 4,
        OPERATION_POWER = 5,
        OPERATION_FACTORIAL = 6
    };

    struct ApplicationContext {
        long long first_number;
        long long second_number;
        Operation operation;
        long long result;
        safe_math::ErrorCode operation_status;
        int help_requested;
        int parse_status;
    };
    
}