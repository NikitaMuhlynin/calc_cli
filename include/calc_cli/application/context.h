#pragma once

#include <safe_math/safe_math.hpp>

#include <cstdint>
#include <unordered_map>

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

    OperationTraits(OperationTraits&& other) = delete;
    OperationTraits& operator=(OperationTraits&& other) = delete;

    static std::string toString(Operation operation);
    static Operation fromString(std::string operationName);
    static std::string makeNormalizedKey(ApplicationContext context);

private:
    static const std::unordered_map<Operation, std::string>& operationToStringMap();
    static const std::unordered_map<std::string, Operation>& stringToOperationMap();

};

}