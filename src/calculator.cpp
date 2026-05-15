#include "calc_cli/calculator.h"

#include <safe_math/safe_math.hpp>

#include <stdexcept>

namespace calc_cli {

void Calculator::calculate(ApplicationContext& request) {
    
    if (request.operation == Operation::Add) {
        request.result = SafeMath::add(request.left, request.right);
        return;
    }
    if (request.operation == Operation::Subtract) {
        request.result = SafeMath::subtract(request.left, request.right);
        return;
    }
    if (request.operation == Operation::Multiply) {
        request.result = SafeMath::multiply(request.left, request.right);
        return;
    }
    if (request.operation == Operation::Divide) {
        request.result = SafeMath::divide(request.left, request.right);
        return;
    }
    if (request.operation == Operation::Power) {
        request.result = SafeMath::power(request.left, request.right);
        return;
    }
    if (request.operation == Operation::Factorial) {
        request.result = SafeMath::factorial(request.left);
        return;
    }

    throw std::logic_error("Error: unsupported operation");
}

}