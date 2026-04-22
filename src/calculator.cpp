#include "calc_cli/calculator.h"

#include <safe_math/safe_math.hpp>

namespace calc_cli {

    void calculate(ApplicationContext* context) {
        safe_math::Result result = safe_math::make_result(0, safe_math::ERROR_OK);

        switch (context->operation) {
        case OPERATION_ADD:
            result = safe_math::add(context->first_number, context->second_number);
            break;

        case OPERATION_SUBTRACT:
            result = safe_math::subtract(context->first_number, context->second_number);
            break;

        case OPERATION_MULTIPLY:
            result = safe_math::multiply(context->first_number, context->second_number);
            break;

        case OPERATION_DIVIDE:
            result = safe_math::divide(context->first_number, context->second_number);
            break;

        case OPERATION_POWER:
            result = safe_math::power(context->first_number, context->second_number);
            break;

        case OPERATION_FACTORIAL:
            result = safe_math::factorial(context->first_number);
            break;

        case OPERATION_NONE:
        default:
            result = safe_math::make_result(0, safe_math::ERROR_NEGATIVE_ARGUMENT);
            break;
        }

        context->result = result.value;
        context->operation_status = result.error;
    }

}