#include "calc_cli/application/context.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

namespace calc_cli {

std::string OperationTraits::toString(Operation operation) {
    const auto& map = operationToStringMap();

    auto iterator = map.find(operation);

    if (iterator == map.end()) {
       throw std::invalid_argument("Error: unknown operation");
    }

    return iterator->second;
}

Operation OperationTraits::fromString(std::string operationName) {
    const auto& map = stringToOperationMap();

    auto iterator = map.find(operationName);

    if (iterator == map.end()) {
        throw std::invalid_argument("Error: unknown operation");
    }

    return iterator->second;
}

std::string OperationTraits::makeNormalizedKey(ApplicationContext context) {
    if (
        context.operation == Operation::Add ||
        context.operation == Operation::Multiply
    ) {
        if (context.left > context.right) {
            std::swap(context.left, context.right);
        }
    }

    if (context.operation == Operation::Factorial) {
        return OperationTraits::toString(context.operation) + ":" +
            std::to_string(context.left);
    }

    return OperationTraits::toString(context.operation) + ":" +
        std::to_string(context.left) + ":" +
        std::to_string(context.right);
}

const std::unordered_map<Operation, std::string>& OperationTraits::operationToStringMap() {
    static const std::unordered_map<Operation, std::string> map = {
        {Operation::Add, "add"},
        {Operation::Subtract, "subtract"},
        {Operation::Multiply, "multiply"},
        {Operation::Divide, "divide"},
        {Operation::Power, "power"},
        {Operation::Factorial, "factorial"}
    };
    
    return map;
}

const std::unordered_map<std::string, Operation>& OperationTraits::stringToOperationMap() {
    static const std::unordered_map<std::string, Operation> map = {
        {"add", Operation::Add},
        {"subtract", Operation::Subtract},
        {"multiply", Operation::Multiply},
        {"divide", Operation::Divide},
        {"power", Operation::Power},
        {"factorial", Operation::Factorial}
    };
    
    return map;
}

}