#pragma once

#include "calc_cli/context.h"

namespace calc_cli {

class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;
    
    Calculator(const Calculator&) = default;
    Calculator& operator=(const Calculator&) = default;

    Calculator(Calculator&&) noexcept = default;
    Calculator& operator=(Calculator&&) noexcept = default;

    void calculate(ApplicationContext& request);

};

}