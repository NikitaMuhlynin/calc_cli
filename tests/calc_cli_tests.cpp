#include <gtest/gtest.h>

#include <stdexcept>
#include <functional>
#include <string>

#include "../include/calc_cli/runner.h"

std::string captureStdout(const std::function<void()>& func) {
    std::ostringstream buffer;
    std::streambuf* oldBuf = std::cout.rdbuf(buffer.rdbuf());
    
    try {
        func();
    } catch (...) {
        std::cout.rdbuf(oldBuf);
        throw;
    }
    
    std::cout.rdbuf(oldBuf);
    return buffer.str();
}

std::string captureStderr(const std::function<void()>& func) {
    std::ostringstream buffer;
    std::streambuf* oldBuf = std::cerr.rdbuf(buffer.rdbuf());
    
    try {
        func();
    } catch (...) {
        std::cerr.rdbuf(oldBuf);
        throw;
    }
    
    std::cerr.rdbuf(oldBuf);
    return buffer.str();
}

// Regular calls

TEST(CalcCliTest, HelpRequested) {
    char programName[] = "calc_cli";
    char helpArg[] = "{\"help\": true}";
    char* argv[] = { programName, helpArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected =
        "Usage:\n"
        "  calc_cli '<json>'\n"
        "  calc_cli '{\"help\": true}'\n\n"
        "Description:\n"
        "  calc_cli performs safe integer arithmetic using JSON input.\n\n"
        "Supported operations:\n"
        "  add, subtract, multiply, divide, power, factorial\n\n"
        "JSON format:\n"
        "  Binary operations:\n"
        "    {\"left\": <int>, \"right\": <int>, \"operation\": \"add\"}\n"
        "  Factorial:\n"
        "    {\"left\": <int>, \"operation\": \"factorial\"}\n"
        "  Help:\n"
        "    {\"help\": true}\n\n"
        "Examples:\n"
        "  calc_cli '{\"left\": 10, \"right\": 20, \"operation\": \"add\"}'\n"
        "  calc_cli '{\"left\": 6, \"operation\": \"factorial\"}'\n"
        "  calc_cli '{\"help\": true}'\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalAdd) {
    char programName[] = "calc_cli";
    char AddArg[] = "{\"left\": 1, \"right\": 2, \"operation\": \"add\"}";
    char* argv[] = { programName, AddArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 3\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalSub) {
    char programName[] = "calc_cli";
    char SubArg[] = "{\"left\": 3, \"right\": 2, \"operation\": \"subtract\"}";
    char* argv[] = { programName, SubArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 1\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalMul) {
    char programName[] = "calc_cli";
    char MulArg[] = "{\"left\": 3, \"right\": 2, \"operation\": \"multiply\"}";
    char* argv[] = { programName, MulArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 6\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalDiv) {
    char programName[] = "calc_cli";
    char DivArg[] = "{\"left\": 8, \"right\": 2, \"operation\": \"divide\"}";
    char* argv[] = { programName, DivArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 4\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalPow) {
    char programName[] = "calc_cli";
    char PowArg[] = "{\"left\": 8, \"right\": 2, \"operation\": \"power\"}";
    char* argv[] = { programName, PowArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 64\n";

    EXPECT_EQ(output, expected);
}

TEST(CalcCliTest, NormalFact) {
    char programName[] = "calc_cli";
    char FactArg[] = "{\"left\": 6, \"operation\": \"factorial\"}";
    char* argv[] = { programName, FactArg };

    std::string output = captureStdout([&]() {
        calc_cli::Runner::run(2, argv);
    });

    std::string expected = "Result: 720\n";

    EXPECT_EQ(output, expected);
}

// Invalid calls

TEST(CalcCliTest, InvalidJSON) {
    char programName[] = "calc_cli";
    char badJson[] = "{\"help\"}";
    char* argv[] = { programName, badJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: invalid JSON input"), std::string::npos);
}

TEST(CalcCliTest, EmptyCall) {
    char programName[] = "calc_cli";
    char* argv[] = { programName };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(1, argv);
    });
    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: JSON input is missing"), std::string::npos);
}

TEST(CalcCliTest, InvalidOperation) {
    char programName[] = "calc_cli";
    char badJson[] = "{\"left\": 16, \"right\": 2, \"operation\": \"squareroot\"}";
    char* argv[] = { programName, badJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: unknown operation"), std::string::npos);
}

TEST(CalcCliTest, LeftIsMissing) {
    char programName[] = "calc_cli";
    char missedLeftJson[] = "{\"right\": 16, \"operation\": \"add\"}";
    char* argv[] = { programName, missedLeftJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: field 'left' is required"), std::string::npos);
}

TEST(CalcCliTest, RightIsMissing) {
    char programName[] = "calc_cli";
    char missedRightJson[] = "{\"left\": 16, \"operation\": \"add\"}";
    char* argv[] = { programName, missedRightJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: field 'right' is required"), std::string::npos);
}

TEST(CalcCliTest, OperationIsMissing) {
    char programName[] = "calc_cli";
    char missedOperationJson[] = "{\"left\": 16, \"right\": \"2\"}";
    char* argv[] = { programName, missedOperationJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: field 'operation' is required"), std::string::npos);
}

TEST(CalcCliTest, DivisionByZero) {
    char programName[] = "calc_cli";
    char divZeroJson[] = "{\"left\": 16, \"right\": 0, \"operation\": \"divide\"}";
    char* argv[] = { programName, divZeroJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: division by zero"), std::string::npos);
}

TEST(CalcCliTest, Overflow) {
    char programName[] = "calc_cli";
    char overflowJson[] = "{\"left\": 21, \"operation\": \"factorial\"}";
    char* argv[] = { programName, overflowJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: multiplication overflow"), std::string::npos);
}

TEST(CalcCliTest, NegativePow) {
    char programName[] = "calc_cli";
    char negativePowJson[] = "{\"left\": 2, \"right\": -2, \"operation\": \"power\"}";
    char* argv[] = { programName, negativePowJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: negative exponent"), std::string::npos);
}

TEST(CalcCliTest, NegativeFactorial) {
    char programName[] = "calc_cli";
    char negativeFactJson[] = "{\"left\": -5, \"operation\": \"factorial\"}";
    char* argv[] = { programName, negativeFactJson };

    int returnCode = 0;
    std::string errorOutput = captureStderr([&]() {
        returnCode = calc_cli::Runner::run(2, argv);
    });

    
    EXPECT_EQ(returnCode, 1);
    EXPECT_NE(errorOutput.find("Error: negative factorial argument"), std::string::npos);
}