#include "Workbench/Commands.hpp"
#include "NoLib/Core/Utility.hpp"
#include <iostream>

namespace
{
    unsigned passed = 0;
    unsigned failed = 0;

    void check(bool condition, const char* name)
    {
        if (condition) ++passed;
        else { ++failed; std::cerr << "FAIL: " << name << '\n'; }
    }

    void value(const char* input, NoLib::Real x, NoLib::Real expected)
    {
        Workbench::ParsedExpression parsed;
        Workbench::Diagnostic error;
        NoLib::Real actual = 0;
        const bool ok = Workbench::parseExpression(input, parsed, error) && Workbench::evaluate(parsed, x, actual, error);
        const NoLib::Real scale = NoLib::absolute(expected) > 1.0e-200L ? NoLib::absolute(expected) : 1.0L;
        check(ok && NoLib::absolute(actual - expected) <= 1.0e-10L * scale, input);
    }

    void rejects(const char* input)
    {
        Workbench::ParsedExpression parsed;
        Workbench::Diagnostic error;
        check(!Workbench::parseExpression(input, parsed, error) && !error.message.empty(), input);
    }

    void commandResult(const char* input, const char* expected)
    {
        Workbench::Command command;
        Workbench::Result result;
        Workbench::Diagnostic error;
        const bool ok = Workbench::parseCommand(input, command, error) && Workbench::calculate(command, result, error);
        if (!ok) std::cerr << "  " << error.message.c_str() << '\n';
        else if (!Workbench::equalText(result.text, expected)) std::cerr << "  Actual: " << result.text.c_str() << '\n';
        check(ok && Workbench::equalText(result.text, expected), input);
    }

    void commandRejects(const char* input)
    {
        Workbench::Command command;
        Workbench::Result result;
        Workbench::Diagnostic error;
        const bool parsed = Workbench::parseCommand(input, command, error);
        check(!parsed || !Workbench::calculate(command, result, error), input);
    }

    void commandValue(const char* input, NoLib::Real x, NoLib::Real expected)
    {
        Workbench::Command command;
        Workbench::Result result;
        Workbench::Diagnostic error;
        Workbench::ParsedExpression parsed;
        NoLib::Real actual = 0;
        const bool ok = Workbench::parseCommand(input, command, error)
            && Workbench::calculate(command, result, error)
            && Workbench::parseExpression(result.text, parsed, error)
            && Workbench::evaluate(parsed, x, actual, error);
        check(ok && NoLib::absolute(actual - expected) <= NoLib::absolute(expected) * 1e-9L, input);
    }
}

int main()
{
    value("x^3 + 2x + 1", 2, 13);
    value("2(x+1)(x-1)", 3, 16);
    value("-x^2", 3, -9);
    value("(-x)^2", 3, 9);
    value("2^3^2", 0, 512);
    value("(2^3)^2", 0, 64);
    value("2^-2", 0, .25L);
    value("x^(1+2)", 2, 8);
    value("x^(-2)", 2, .25L);
    value("1.5e2 + .25", 0, 150.25L);
    value("1e-15*x", 3, 3e-15L);
    value("1e200^-1", 0, 1e-200L);
    value("sin(pi/2)", 0, 1);
    value("COS(0)", 0, 1);
    value("tan(0)", 0, 0);
    value("atan(1)", 0, .7853981633974483096L);
    value("ln(e)", 0, 1);
    value("lnabs(-1)", 0, 0);
    value("exp(0)", 0, 1);
    value("2sin(x)", 0, 0);
    value("6/2x", 4, 12);
    value("6/(2x)", 3, 1);
    value("2 * -3 + +4", 0, -2);
    value("x - (2 - x)", 5, 8);
    value("x / (2 / x)", 4, 8);
    value("x(x+1)", 2, 6);
    value("\tX ^ 3 + 2 X\r\n", 2, 12);

    const char* invalid[] = { "", "x^", "x^0.5", "x^x", "x^1025", "x^-1025", "2^^3", "(x+1", "x+1)",
        "x+", "1..2", "1.2.3", ".", "1e", "1e+", "1e-", "2 3", "xx", "y+1", "sin x", "sine(x)",
        "sin()", "sin(x,2)", "[x]", "x=2", "2**3", "1/0", "0^0", "0^-1", "ln(-1)", "ln(0)", "lnabs(0)",
        "sin(1e20)", "exp(1e20)", "2e", "0*(1/0)", "(1/0)^0" };
    for (const char* input : invalid) rejects(input);

    Workbench::ParsedExpression parsed;
    Workbench::Diagnostic error;
    NoLib::Real result = 0;
    check(Workbench::parseExpression("ln(x)", parsed, error) && !Workbench::evaluate(parsed, -1, result, error), "ln(x) domain");
    check(Workbench::parseExpression("0*(1/x)", parsed, error) && !Workbench::evaluate(parsed, 0, result, error), "no zero-product domain erasure");
    check(Workbench::parseExpression("x^0", parsed, error) && !Workbench::evaluate(parsed, 0, result, error), "no zero-power domain erasure");

    commandResult("action[dirive(x^3 + 2x + 1)]", "3x^2 + 2");
    commandResult("action[derive(x^3 + 2x + 1)]", "3x^2 + 2");
    commandResult("  AcTiOn [ DiFf( X^3 + 2X + 1 ) ]  ", "3x^2 + 2");
    commandResult("action[eval(x^3 + 2x + 1, 2)]", "13");
    commandResult("action[integrate(3x^2 + 2)]", "x^3 + 2x + C");
    commandResult("action[simplify((x+1)(x-1))]", "x^2 - 1");
    commandResult("derive(sin(x))", "cos(x)");
    commandResult("2x + 3x + 1", "5x + 1");
    commandResult("eval(2^3^2)", "512");
    commandResult("eval(sin(x), pi/2)", "1");
    commandResult("eval(1.5e2 + .25)", "150.25");
    commandResult("derive(7)", "0");
    commandResult("integrate(0)", "0 + C");
    commandResult("derive((x+1)^3)", "3x^2 + 6x + 3");
    commandValue("simplify(x/1e20)", 2, 2e-20L);
    commandValue("derive(1e-15*x^2)", 3, 6e-15L);
    commandValue("simplify(1e-15*x + 2e-15*x)", 2, 6e-15L);
    commandValue("derive(sin(x^2))", 0.5L, .9689124217106447841L);
    commandValue("derive(ln(x))", 2, .5L);
    commandValue("derive((x+1)/(x-1))", 3, -.5L);
    commandValue("derive(exp(2x))", 0, 2);

    const char* badCommands[] = { "action[wat(x)]", "action[derive()]", "action[derive(x)", "action derive(x)",
        "action[derive(x)]junk", "action[derive(x),eval(x,2)]", "derive(x,2)", "eval(x)", "eval(x,x)", "eval(x,)",
        "eval(,2)", "eval(x,2,3)", "derive(x))", "action[]", "derive", "eval(ln(x),-1)", "eval(1/(x-2),2)",
        "integrate(sin(1e100*x))" };
    for (const char* input : badCommands) commandRejects(input);

    // Exact command-to-source position mapping, including wrapper and spaces.
    Workbench::Command command;
    Workbench::Result output;
    check(Workbench::parseCommand("action[derive(x^)]", command, error)
        && !Workbench::calculate(command, output, error) && error.position == 16, "caret position in action");
    check(Workbench::parseCommand("action[help()]", command, error) && command.kind == Workbench::CommandKind::Help, "wrapped help");
    check(Workbench::parseCommand("help", command, error) && command.kind == Workbench::CommandKind::Help, "bare help");

    // Round-trip grouping of powers, quotients and signed bases.
    const char* roundTrips[] = { "(x^2)^3", "x/(2/x)", "x-(2-x)", "(-2)^2", "x*(2/x)", "ln(x)", "1e-15*x" };
    for (const char* input : roundTrips)
    {
        Workbench::ParsedExpression second;
        NoLib::Real before = 0, after = 0;
        const bool ok = Workbench::parseExpression(input, parsed, error)
            && Workbench::evaluate(parsed, 3, before, error)
            && Workbench::parseExpression(Workbench::formatExpression(parsed.tree, parsed.strictLog), second, error)
            && Workbench::evaluate(second, 3, after, error);
        check(ok && NoLib::absolute(before - after) < 1.0e-10L, input);
    }

    NoLib::String deep;
    for (unsigned i = 0; i < 200; ++i) deep += '(';
    deep += 'x';
    for (unsigned i = 0; i < 200; ++i) deep += ')';
    check(!Workbench::parseExpression(deep, parsed, error), "deep parentheses bounded");
    NoLib::String signs;
    for (unsigned i = 0; i < 200; ++i) signs += '-';
    signs += 'x';
    check(!Workbench::parseExpression(signs, parsed, error), "deep signs bounded");
    NoLib::String longLine;
    for (unsigned i = 0; i < 2050; ++i) longLine += 'x';
    check(!Workbench::parseCommand(longLine, command, error), "line length bounded");
    NoLib::String embeddedNull("x");
    embeddedNull += '\0';
    embeddedNull += "+2";
    check(!Workbench::parseExpression(embeddedNull, parsed, error), "embedded NUL cannot truncate an expression");

    // Deterministic malformed-input stress test. No external test framework.
    unsigned seed = 541;
    const char alphabet[] = "x12.+-*/^(),[] sincoet";
    for (unsigned iteration = 0; iteration < 3000; ++iteration)
    {
        NoLib::String fuzz;
        seed = seed * 1664525U + 1013904223U;
        const unsigned length = seed % 120;
        for (unsigned i = 0; i < length; ++i)
        {
            seed = seed * 1664525U + 1013904223U;
            fuzz += alphabet[seed % (sizeof(alphabet) - 1)];
        }
        const bool ok = Workbench::parseExpression(fuzz, parsed, error);
        check(ok || !error.message.empty(), "malformed-input diagnostic");
    }

    std::cout << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
