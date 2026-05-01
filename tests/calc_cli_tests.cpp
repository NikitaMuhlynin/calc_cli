#include <gtest/gtest.h>

#include <stdexcept>
#include <functional>
#include <string>
#include <iosfwd>

#include "../include/calc_cli/runner.h"

struct RunResult {
    int code;
    std::string out;
    std::string err;
};

RunResult runCli(int argc, char** argv) {
    std::ostringstream out;
    std::ostringstream err;

    calc_cli::Runner runner(out, err);
    int code = runner.run(argc, argv);

    return RunResult{code, out.str(), err.str()};
}

// Regular calls

TEST(CalcCliTest, HelpRequested) {
    char programName[] = "calc_cli";
    char helpArg[] = "{\"help\": true}";
    char* argv[] = { programName, helpArg };

    RunResult output = runCli(2, argv);

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

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalAdd) {
    char programName[] = "calc_cli";
    char AddArg[] = "{\"left\": 1, \"right\": 2, \"operation\": \"add\"}";
    char* argv[] = { programName, AddArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 3\n";

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalSub) {
    char programName[] = "calc_cli";
    char SubArg[] = "{\"left\": 3, \"right\": 2, \"operation\": \"subtract\"}";
    char* argv[] = { programName, SubArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 1\n";

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalMul) {
    char programName[] = "calc_cli";
    char MulArg[] = "{\"left\": 3, \"right\": 2, \"operation\": \"multiply\"}";
    char* argv[] = { programName, MulArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 6\n";

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalDiv) {
    char programName[] = "calc_cli";
    char DivArg[] = "{\"left\": 8, \"right\": 2, \"operation\": \"divide\"}";
    char* argv[] = { programName, DivArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 4\n";

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalPow) {
    char programName[] = "calc_cli";
    char PowArg[] = "{\"left\": 8, \"right\": 2, \"operation\": \"power\"}";
    char* argv[] = { programName, PowArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 64\n";

    EXPECT_EQ(output.out, expected);
}

TEST(CalcCliTest, NormalFact) {
    char programName[] = "calc_cli";
    char FactArg[] = "{\"left\": 6, \"operation\": \"factorial\"}";
    char* argv[] = { programName, FactArg };

    RunResult output = runCli(2, argv);

    std::string expected = "Result: 720\n";

    EXPECT_EQ(output.out, expected);
}

// Invalid calls

TEST(CalcCliTest, InvalidJSON) {
    char programName[] = "calc_cli";
    char badJson[] = "{\"help\"}";
    char* argv[] = { programName, badJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: invalid JSON input"), std::string::npos);
}

TEST(CalcCliTest, EmptyCall) {
    char programName[] = "calc_cli";
    char* argv[] = { programName };

    RunResult output = runCli(1, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: JSON input is missing"), std::string::npos);
}

TEST(CalcCliTest, InvalidOperation) {
    char programName[] = "calc_cli";
    char badJson[] = "{\"left\": 16, \"right\": 2, \"operation\": \"squareroot\"}";
    char* argv[] = { programName, badJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: unknown operation"), std::string::npos);
}

TEST(CalcCliTest, LeftIsMissing) {
    char programName[] = "calc_cli";
    char missedLeftJson[] = "{\"right\": 16, \"operation\": \"add\"}";
    char* argv[] = { programName, missedLeftJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: field 'left' is required"), std::string::npos);
}

TEST(CalcCliTest, RightIsMissing) {
    char programName[] = "calc_cli";
    char missedRightJson[] = "{\"left\": 16, \"operation\": \"add\"}";
    char* argv[] = { programName, missedRightJson };

    RunResult output = runCli(2, argv);

    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: field 'right' is required"), std::string::npos);
}

TEST(CalcCliTest, OperationIsMissing) {
    char programName[] = "calc_cli";
    char missedOperationJson[] = "{\"left\": 16, \"right\": \"2\"}";
    char* argv[] = { programName, missedOperationJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: field 'operation' is required"), std::string::npos);
}

TEST(CalcCliTest, DivisionByZero) {
    char programName[] = "calc_cli";
    char divZeroJson[] = "{\"left\": 16, \"right\": 0, \"operation\": \"divide\"}";
    char* argv[] = { programName, divZeroJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: division by zero"), std::string::npos);
}

TEST(CalcCliTest, Overflow) {
    char programName[] = "calc_cli";
    char overflowJson[] = "{\"left\": 21, \"operation\": \"factorial\"}";
    char* argv[] = { programName, overflowJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: multiplication overflow"), std::string::npos);
}

TEST(CalcCliTest, NegativePow) {
    char programName[] = "calc_cli";
    char negativePowJson[] = "{\"left\": 2, \"right\": -2, \"operation\": \"power\"}";
    char* argv[] = { programName, negativePowJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: negative exponent"), std::string::npos);
}

TEST(CalcCliTest, NegativeFactorial) {
    char programName[] = "calc_cli";
    char negativeFactJson[] = "{\"left\": -5, \"operation\": \"factorial\"}";
    char* argv[] = { programName, negativeFactJson };

    RunResult output = runCli(2, argv);
    
    EXPECT_EQ(output.code, 1);
    EXPECT_NE(output.err.find("Error: negative factorial argument"), std::string::npos);
}