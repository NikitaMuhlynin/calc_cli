#pragma once

#include <safe_math/safe_math.hpp>

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <string>

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

class OperationTraits final {
public:
    OperationTraits() = delete;
    ~OperationTraits() = default;

    OperationTraits(const OperationTraits& other) = delete;
    OperationTraits& operator=(const OperationTraits& other) = delete;

    OperationTraits(OperationTraits&& other) noexcept = delete;
    OperationTraits& operator=(OperationTraits&& other) noexcept = delete;

    inline static std::string toString(Operation operation) {
        const auto& map = operationToStringMap();

        auto iterator = map.find(operation);

        if (iterator == map.end()) {
        throw std::invalid_argument("Error: unknown operation");
        }

        return iterator->second;
    }

    inline static Operation fromString(std::string operationName) {
        const auto& map = stringToOperationMap();

        auto iterator = map.find(operationName);

        if (iterator == map.end()) {
            throw std::invalid_argument("Error: unknown operation");
        }

        return iterator->second;
    }

    inline static std::string makeNormalizedKey(ApplicationContext context) {
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

private:
    inline static const std::unordered_map<Operation, std::string>& operationToStringMap() {
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

    inline static const std::unordered_map<std::string, Operation>& stringToOperationMap() {
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

};

}