#pragma once

#include "calc_cli/context.h"

namespace calc_cli {

class Calculator {
public:
    Calculator() = delete;

    static long long calculate(const CalculationRequest& request);

};

}