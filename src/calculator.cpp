#include "calc_cli/calculator.h"

#include <safe_math/safe_math.hpp>

#include <stdexcept>

namespace calc_cli {

long long Calculator::calculate(const CalculationRequest& request) {
    
    if (request.operation == Operation::Add) {
        return SafeMath::add(request.left, *request.right);
    }
    if (request.operation == Operation::Subtract) {
        return SafeMath::subtract(request.left, *request.right);
    }
    if (request.operation == Operation::Multiply) {
        return SafeMath::multiply(request.left, *request.right);
    }
    if (request.operation == Operation::Divide) {
        return SafeMath::divide(request.left, *request.right);
    }
    if (request.operation == Operation::Power) {
        return SafeMath::power(request.left, *request.right);
    }
    if (request.operation == Operation::Factorial) {
        return SafeMath::factorial(request.left);
    }

    throw std::logic_error("Error: unsupported operation");
}

}