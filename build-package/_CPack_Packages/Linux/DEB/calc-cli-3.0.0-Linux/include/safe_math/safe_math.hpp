#pragma once

#include <stdexcept>
#include <limits>

namespace SafeMath {

static long long add(long long left, long long right) {
    long long result = 0;
    
    if (__builtin_add_overflow(left, right, &result))
        throw std::overflow_error("Error: addition overflow");

    return result;
}

static long long subtract(long long left, long long right) {
    long long result = 0;

    if (__builtin_sub_overflow(left, right, &result)) 
        throw std::overflow_error("Error: subtraction overflow");

    return result;
}

static long long multiply(long long left, long long right) {
    long long result = 0;

    if (__builtin_mul_overflow(left, right, &result))
        throw std::overflow_error("Error: multiplication overflow");
    
    return result;
}

static long long divide(long long left, long long right) {
    if (right == 0) 
        throw std::invalid_argument("Error: division by zero");

    if (left == std::numeric_limits<long long>::min() && right == -1)
        throw std::overflow_error("Error: division overflow");

    return left / right;
}

static long long power(long long base, long long exponent) {
    long long result = 1;
    long long current_base = base;
    long long current_exponent = exponent;

    if (current_exponent < 0) 
        throw std::invalid_argument("Error: negative exponent");

    while (current_exponent > 0) {
        if (current_exponent % 2 != 0) 
            result = multiply(result, current_base);
        
        current_exponent /= 2;

        if (current_exponent > 0) {
            long long square = multiply(current_base, current_base);
            current_base = square;
        }
    }

    return result;
}

namespace {

static long long factorial_step(long long current, long long target, long long accumulated) {
    if (current > target)
        return accumulated;
    
    long long multiplication = multiply(accumulated, current);

    return factorial_step(current + 1, target, multiplication);
}

} // anonimous namespace

static long long factorial(long long value) {
    if (value < 0)
        throw std::invalid_argument("Error: negative factorial argument");
    
    return factorial_step(1, value, 1);
}

} // SafaMath namespace