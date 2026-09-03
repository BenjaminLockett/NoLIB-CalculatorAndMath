#include "Workbench/ExpressionParser.hpp"
#include "NoLib/Text/NumberParsing.hpp"
#include "NoLib/Scalar/Constants.hpp"
#include "NoLib/Scalar/Powers.hpp"
#include "NoLib/Scalar/Exponential.hpp"
#include "NoLib/Scalar/Logarithm.hpp"
#include "NoLib/Scalar/Trigonometry.hpp"
#include "NoLib/Core/Utility.hpp"

namespace Workbench
{
    using NoLib::UInt;
    using NoLib::ULong;
    using NoLib::Real;
    using NoLib::String;

    bool equalText(const String& text, const char* literal)
    {
        ULong i = 0;
        while (literal[i] != '\0')
        {
            if (i >= text.length() || text[i] != literal[i]) return false;
            ++i;
        }
        return i == text.length();
    }

    String slice(const String& text, ULong begin, ULong end)
    {
        String result;
        for (ULong i = begin; i < end && i < text.length(); ++i) result += text[i];
        return result;
    }

    bool finite(Real value)
    {
        const Real zero = value * 0.0L;
        return value == value && zero == zero;
    }

    namespace
    {
        bool digit(char c) { return c >= '0' && c <= '9'; }
        bool letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
        char lower(char c) { return c >= 'A' && c <= 'Z' ? static_cast<char>(c + ('a' - 'A')) : c; }

        // All intermediate operations are checked before they reach the legacy
        // algorithms. In particular, zero denominators cannot be simplified away.
        bool apply(const NoLib::SymbolicNode& node, bool strictLog, Real a, Real b, Real& value, String& error)
        {
            bool ok = true;
            switch (node.type_SymbolicType_Var)
            {
            case NoLib::SymbolicAdd: value = a + b; break;
            case NoLib::SymbolicSubtract: value = a - b; break;
            case NoLib::SymbolicMultiply: value = a * b; break;
            case NoLib::SymbolicDivide:
                if (b == 0.0L) { error = "Division by zero."; return false; }
                value = a / b;
                break;
            case NoLib::SymbolicIntegerPower:
                if (a == 0.0L && node.exponent_SLong_Var <= 0)
                {
                    error = "Zero cannot have a zero or negative exponent.";
                    return false;
                }
                // Invert first for negative powers: avoids overflow in a^n when
                // a^(-n) is representable, without modifying realIntegerPower.
                ok = NoLib::realIntegerPower(node.exponent_SLong_Var < 0 ? 1.0L / a : a,
                    node.exponent_SLong_Var < 0 ? -node.exponent_SLong_Var : node.exponent_SLong_Var, value);
                break;
            case NoLib::SymbolicLogAbsolute:
                if ((strictLog && a <= 0.0L) || a == 0.0L)
                {
                    error = strictLog ? "ln requires a positive argument." : "lnabs requires a nonzero argument.";
                    return false;
                }
                ok = NoLib::naturalLog(NoLib::absolute(a), value);
                break;
            case NoLib::SymbolicExponential: ok = NoLib::exponential(a, value); break;
            case NoLib::SymbolicSine:
            case NoLib::SymbolicCosine:
                if (NoLib::absolute(a) > 1000000.0L)
                {
                    error = "Trig arguments must be within +/-1000000 radians.";
                    return false;
                }
                value = node.type_SymbolicType_Var == NoLib::SymbolicSine ? NoLib::sine(a) : NoLib::cosine(a);
                break;
            case NoLib::SymbolicArcTangent: value = NoLib::arcTangent(a); break;
            default: error = "Unsupported expression node."; return false;
            }
            if (!ok || !finite(value))
            {
                error = "Result is outside the supported real-number range.";
                return false;
            }
            return true;
        }

        class Parser
        {
            const String& text;
            ParsedExpression& output;
            Diagnostic& error;
            ULong at = 0;
            UInt recursion = 0;

            bool fail(const char* message, ULong position)
            {
                if (error.message.empty()) { error.message = message; error.position = position; }
                return false;
            }
            void spaces()
            {
                while (at < text.length() && (text[at] == ' ' || text[at] == '\t' || text[at] == '\r' || text[at] == '\n')) ++at;
            }
            char peek() { spaces(); return at < text.length() ? text[at] : '\0'; }
            bool take(char c) { if (peek() != c) return false; ++at; return true; }

            bool add(NoLib::SymbolicType type, UInt left, UInt right, Real value, NoLib::SLong exponent,
                bool strict, ULong position, UInt& result)
            {
                const bool leaf = type == NoLib::SymbolicConstant || type == NoLib::SymbolicVariable;
                const bool binary = type >= NoLib::SymbolicAdd && type <= NoLib::SymbolicDivide;
                const UInt depth = leaf ? 1 : 1 + (binary ? NoLib::maximum(output.depth[left], output.depth[right]) : output.depth[left]);
                if (depth > DepthLimit) return fail("Expression is too deeply nested; split it into smaller expressions.", position);
                NoLib::SymbolicNode node{type, value, exponent, left, right};
                if (!output.tree.addNode(node, result)) return fail("Expression is too large (512 nodes maximum).", position);
                output.position[result] = position;
                output.strictLog[result] = strict;
                output.depth[result] = depth;
                output.constant[result] = type == NoLib::SymbolicConstant || (!leaf && output.constant[left] && (!binary || output.constant[right]));
                if (type == NoLib::SymbolicConstant) output.value[result] = value;
                else if (output.constant[result])
                {
                    if (!apply(node, strict, output.value[left], binary ? output.value[right] : 0.0L, output.value[result], error.message))
                    {
                        error.position = position;
                        return false;
                    }
                }
                return true;
            }
            bool number(UInt& result)
            {
                const ULong start = at;
                bool digits = false;
                while (at < text.length() && digit(text[at])) { ++at; digits = true; }
                if (at < text.length() && text[at] == '.')
                {
                    ++at;
                    while (at < text.length() && digit(text[at])) { ++at; digits = true; }
                }
                if (!digits) return fail("Expected a number, x, a function, or '('.", start);
                if (at < text.length() && (text[at] == 'e' || text[at] == 'E'))
                {
                    // 2e3 is scientific notation; use 2*e for Euler's constant.
                    const ULong exponentStart = at++;
                    if (at < text.length() && (text[at] == '+' || text[at] == '-')) ++at;
                    const ULong firstDigit = at;
                    while (at < text.length() && digit(text[at])) ++at;
                    if (at == firstDigit) return fail("Scientific notation needs exponent digits; multiply Euler's constant with 2*e.", exponentStart);
                }
                if (at < text.length() && text[at] == '.') return fail("A number can contain only one decimal point.", at);
                Real value = 0.0L;
                if (!NoLib::readReal(slice(text, start, at), value) || !finite(value)) return fail("Invalid number or number outside the supported range.", start);
                return add(NoLib::SymbolicConstant, 0, 0, value, 0, false, start, result);
            }
            bool primary(UInt& result)
            {
                const char c = peek();
                const ULong start = at;
                if (take('('))
                {
                    if (!expression(result)) return false;
                    if (!take(')')) return fail("Expected ')' to close this expression.", at);
                    return true;
                }
                if (letter(c))
                {
                    String name;
                    while (at < text.length() && letter(text[at])) name += lower(text[at++]);
                    if (equalText(name, "x")) return add(NoLib::SymbolicVariable, 0, 0, 0, 0, false, start, result);
                    if (equalText(name, "pi")) return add(NoLib::SymbolicConstant, 0, 0, NoLib::piApproximation(), 0, false, start, result);
                    if (equalText(name, "e"))
                    {
                        Real e = 0.0L;
                        NoLib::exponential(1.0L, e);
                        return add(NoLib::SymbolicConstant, 0, 0, e, 0, false, start, result);
                    }
                    NoLib::SymbolicType type;
                    const bool strict = equalText(name, "ln");
                    const bool tangent = equalText(name, "tan");
                    if (equalText(name, "sin") || tangent) type = NoLib::SymbolicSine;
                    else if (equalText(name, "cos")) type = NoLib::SymbolicCosine;
                    else if (equalText(name, "exp")) type = NoLib::SymbolicExponential;
                    else if (strict || equalText(name, "lnabs")) type = NoLib::SymbolicLogAbsolute;
                    else if (equalText(name, "atan")) type = NoLib::SymbolicArcTangent;
                    else return fail("Unknown name. Use x, pi, e, sin, cos, tan, atan, exp, ln, or lnabs.", start);
                    if (!take('(')) return fail("A function needs parentheses, for example sin(x).", at);
                    UInt argument = 0;
                    if (!expression(argument)) return false;
                    if (!take(')')) return fail("Expected ')' after the function argument.", at);
                    if (!add(type, argument, 0, 0, 0, strict, start, result)) return false;
                    if (tangent)
                    {
                        UInt cosine = 0;
                        const UInt sine = result;
                        return add(NoLib::SymbolicCosine, argument, 0, 0, 0, false, start, cosine)
                            && add(NoLib::SymbolicDivide, sine, cosine, 0, 0, false, start, result);
                    }
                    return true;
                }
                return number(result);
            }
            bool power(UInt& result)
            {
                if (!primary(result)) return false;
                if (peek() == '^')
                {
                    const ULong position = at++;
                    UInt exponent = 0;
                    if (!unary(exponent)) return false; // right-associative: 2^3^2 == 512
                    if (!output.constant[exponent]) return fail("The exponent must be a constant integer; use exp(x) for variable exponentials.", position + 1);
                    const Real value = output.value[exponent];
                    if (value < -1024 || value > 1024 || value != static_cast<NoLib::SLong>(value))
                        return fail("Exponents must be integers from -1024 to 1024.", position + 1);
                    const UInt base = result;
                    return add(NoLib::SymbolicIntegerPower, base, 0, 0, static_cast<NoLib::SLong>(value), false, position, result);
                }
                return true;
            }
            bool unary(UInt& result)
            {
                if (++recursion > DepthLimit) { --recursion; return fail("Too many nested parentheses, signs, or powers.", at); }
                const char c = peek();
                const ULong position = at;
                bool ok;
                if (c == '+' || c == '-')
                {
                    ++at;
                    ok = unary(result);
                    if (ok && c == '-')
                    {
                        UInt minusOne = 0;
                        const UInt argument = result;
                        ok = add(NoLib::SymbolicConstant, 0, 0, -1, 0, false, position, minusOne)
                            && add(NoLib::SymbolicMultiply, minusOne, argument, 0, 0, false, position, result);
                    }
                }
                else ok = power(result);
                --recursion;
                return ok;
            }
            bool term(UInt& result)
            {
                if (!unary(result)) return false;
                while (true)
                {
                    const char c = peek();
                    const ULong position = at;
                    const bool implicit = c == '(' || letter(c);
                    if (c != '*' && c != '/' && !implicit) break;
                    if (!implicit) ++at;
                    UInt right = 0;
                    if (!unary(right)) return false;
                    const UInt left = result;
                    if (!add(c == '/' ? NoLib::SymbolicDivide : NoLib::SymbolicMultiply, left, right, 0, 0, false, position, result)) return false;
                }
                return true;
            }
            bool expression(UInt& result)
            {
                if (!term(result)) return false;
                while (peek() == '+' || peek() == '-')
                {
                    const ULong position = at;
                    const char c = text[at++];
                    UInt right = 0;
                    if (!term(right)) return false;
                    const UInt left = result;
                    if (!add(c == '+' ? NoLib::SymbolicAdd : NoLib::SymbolicSubtract, left, right, 0, 0, false, position, result)) return false;
                }
                return true;
            }
        public:
            Parser(const String& source, ParsedExpression& destination, Diagnostic& diagnostic)
                : text(source), output(destination), error(diagnostic) {}
            bool parse()
            {
                output.tree.clear();
                error = Diagnostic{};
                if (text.length() > LineLimit) return fail("Input exceeds 2048 characters.", LineLimit);
                if (!expression(output.tree.root_UInt_Var)) return false;
                spaces();
                if (at != text.length()) return fail("Unexpected character. Use +, -, *, /, ^ and parentheses; only x is a variable.", at);
                return true;
            }
        };
    }

    bool parseExpression(const String& text, ParsedExpression& output, Diagnostic& error)
    {
        Parser parser(text, output, error);
        const bool ok = parser.parse();
        if (!ok) output.tree.clear();
        return ok;
    }

    bool evaluate(const ParsedExpression& expression, Real x, Real& result, Diagnostic& error)
    {
        error = Diagnostic{};
        if (!finite(x) || expression.tree.root_UInt_Var >= expression.tree.count_UInt_Var)
        {
            error.message = "Cannot evaluate an invalid expression or nonfinite x.";
            return false;
        }
        Real values[NodeLimit] = {};
        for (UInt i = 0; i < expression.tree.count_UInt_Var; ++i)
        {
            const auto& node = expression.tree.nodes_SymbolicNodeArray_Var[i];
            if (node.type_SymbolicType_Var == NoLib::SymbolicConstant) values[i] = node.constant_Real_Var;
            else if (node.type_SymbolicType_Var == NoLib::SymbolicVariable) values[i] = x;
            else if (!apply(node, expression.strictLog[i], values[node.left_UInt_Var], values[node.right_UInt_Var], values[i], error.message))
            {
                error.position = expression.position[i];
                return false;
            }
        }
        result = values[expression.tree.root_UInt_Var];
        return true;
    }
}
