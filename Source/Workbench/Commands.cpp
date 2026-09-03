#include "Workbench/Commands.hpp"
#include "NoLib/Symbolic/SymbolicDifferentiation.hpp"
#include "NoLib/Symbolic/PolynomialBridge.hpp"
#include "NoLib/Symbolic/RischSubset.hpp"
#include "NoLib/Polynomial/PolynomialCalculus.hpp"
#include "NoLib/Text/RealFormatting.hpp"
#include "NoLib/Text/IntegerFormatting.hpp"

namespace Workbench
{
    using NoLib::String;
    using NoLib::UInt;
    using NoLib::ULong;
    using NoLib::Real;

    namespace
    {
        bool space(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
        void trim(const String& line, ULong& begin, ULong& end)
        {
            while (begin < end && space(line[begin])) ++begin;
            while (end > begin && space(line[end - 1])) --end;
        }
        String lowercase(String text)
        {
            for (ULong i = 0; i < text.length(); ++i)
                if (text[i] >= 'A' && text[i] <= 'Z') text[i] = static_cast<char>(text[i] + ('a' - 'A'));
            return text;
        }
        bool commandKind(const String& name, CommandKind& kind)
        {
            if (equalText(name, "derive") || equalText(name, "dirive") || equalText(name, "diff")) kind = CommandKind::Derive;
            else if (equalText(name, "integrate") || equalText(name, "int")) kind = CommandKind::Integrate;
            else if (equalText(name, "simplify") || equalText(name, "expand")) kind = CommandKind::Simplify;
            else if (equalText(name, "eval") || equalText(name, "evaluate")) kind = CommandKind::Evaluate;
            else if (equalText(name, "help") || equalText(name, "commands")) kind = CommandKind::Help;
            else if (equalText(name, "examples")) kind = CommandKind::Examples;
            else if (equalText(name, "history")) kind = CommandKind::History;
            else if (equalText(name, "clear") || equalText(name, "cls")) kind = CommandKind::Clear;
            else if (equalText(name, "theme")) kind = CommandKind::Theme;
            else if (equalText(name, "quit") || equalText(name, "exit")) kind = CommandKind::Quit;
            else return false;
            return true;
        }

        UInt precedence(const NoLib::SymbolicNode& node)
        {
            switch (node.type_SymbolicType_Var)
            {
            case NoLib::SymbolicAdd:
            case NoLib::SymbolicSubtract: return 1;
            case NoLib::SymbolicMultiply:
            case NoLib::SymbolicDivide: return 2;
            case NoLib::SymbolicIntegerPower: return 3;
            default: return node.type_SymbolicType_Var == NoLib::SymbolicConstant && node.constant_Real_Var < 0 ? 2 : 4;
            }
        }

        void formatNode(const Expression& tree, UInt index, UInt parent, UInt depth, const bool* strictLog, String& text)
        {
            if (index >= tree.count_UInt_Var || depth > 128 || text.length() > 16000) return;
            const auto& node = tree.nodes_SymbolicNodeArray_Var[index];
            const UInt priority = precedence(node);
            const bool brackets = priority < parent;
            if (brackets) text += '(';
            if (node.type_SymbolicType_Var == NoLib::SymbolicConstant) text += formatNumber(node.constant_Real_Var);
            else if (node.type_SymbolicType_Var == NoLib::SymbolicVariable) text += 'x';
            else if (node.type_SymbolicType_Var >= NoLib::SymbolicAdd && node.type_SymbolicType_Var <= NoLib::SymbolicDivide)
            {
                formatNode(tree, node.left_UInt_Var, priority, depth + 1, strictLog, text);
                switch (node.type_SymbolicType_Var)
                {
                case NoLib::SymbolicAdd: text += " + "; break;
                case NoLib::SymbolicSubtract: text += " - "; break;
                case NoLib::SymbolicMultiply: text += " * "; break;
                default: text += " / "; break;
                }
                // Always preserve right-hand grouping at equal precedence.
                formatNode(tree, node.right_UInt_Var, priority + 1, depth + 1, strictLog, text);
            }
            else if (node.type_SymbolicType_Var == NoLib::SymbolicIntegerPower)
            {
                formatNode(tree, node.left_UInt_Var, priority + 1, depth + 1, strictLog, text);
                text += '^';
                text += NoLib::signedToString(node.exponent_SLong_Var);
            }
            else
            {
                switch (node.type_SymbolicType_Var)
                {
                case NoLib::SymbolicSine: text += "sin("; break;
                case NoLib::SymbolicCosine: text += "cos("; break;
                case NoLib::SymbolicArcTangent: text += "atan("; break;
                case NoLib::SymbolicExponential: text += "exp("; break;
                default: text += strictLog && strictLog[index] ? "ln(" : "lnabs("; break;
                }
                formatNode(tree, node.left_UInt_Var, 0, depth + 1, strictLog, text);
                text += ')';
            }
            if (brackets) text += ')';
        }

        // Collapse only already-validated constant subtrees. Copying through the
        // original builders then supplies their existing local simplifications.
        bool prepare(const ParsedExpression& parsed, Expression& output)
        {
            Expression constants = parsed.tree;
            for (UInt i = 0; i < constants.count_UInt_Var; ++i)
            {
                if (parsed.constant[i])
                {
                    auto& node = constants.nodes_SymbolicNodeArray_Var[i];
                    node.type_SymbolicType_Var = NoLib::SymbolicConstant;
                    node.constant_Real_Var = parsed.value[i];
                }
            }
            output.clear();
            return NoLib::copySymbolicNode(constants, constants.root_UInt_Var, output, output.root_UInt_Var);
        }

        bool validConstants(const Expression& tree)
        {
            for (UInt i = 0; i < tree.count_UInt_Var; ++i)
                if (tree.nodes_SymbolicNodeArray_Var[i].type_SymbolicType_Var == NoLib::SymbolicConstant
                    && !finite(tree.nodes_SymbolicNodeArray_Var[i].constant_Real_Var)) return false;
            return true;
        }

        using Polynomial = NoLib::Polynomial<32>;

        void trimExact(Polynomial& value)
        {
            while (value.degree_UInt_Var > 0 && value.coefficients_RealArray_Var[value.degree_UInt_Var] == 0)
                --value.degree_UInt_Var;
        }

        bool multiply(const Polynomial& left, const Polynomial& right, Polynomial& output)
        {
            if (left.degree_UInt_Var + right.degree_UInt_Var > 32) return false;
            Polynomial product;
            product.degree_UInt_Var = left.degree_UInt_Var + right.degree_UInt_Var;
            for (UInt i = 0; i <= left.degree_UInt_Var; ++i)
            {
                for (UInt j = 0; j <= right.degree_UInt_Var; ++j)
                {
                    Real& coefficient = product.coefficients_RealArray_Var[i + j];
                    coefficient += left.coefficients_RealArray_Var[i] * right.coefficients_RealArray_Var[j];
                    if (!finite(coefficient)) return false;
                }
            }
            trimExact(product);
            output = product;
            return true;
        }

        // An adapter for the original Polynomial type. Unlike the old bridge it
        // removes only exact zero coefficients, never an arbitrary tiny value.
        bool toPolynomial(const Expression& tree, UInt index, Polynomial& output)
        {
            const auto& node = tree.nodes_SymbolicNodeArray_Var[index];
            output = Polynomial{};
            if (node.type_SymbolicType_Var == NoLib::SymbolicConstant)
            {
                output.coefficients_RealArray_Var[0] = node.constant_Real_Var;
                return true;
            }
            if (node.type_SymbolicType_Var == NoLib::SymbolicVariable)
            {
                output.degree_UInt_Var = 1;
                output.coefficients_RealArray_Var[1] = 1;
                return true;
            }
            if (node.type_SymbolicType_Var > NoLib::SymbolicIntegerPower) return false;
            Polynomial left;
            if (!toPolynomial(tree, node.left_UInt_Var, left)) return false;
            if (node.type_SymbolicType_Var == NoLib::SymbolicIntegerPower)
            {
                if (node.exponent_SLong_Var < 0) return false;
                NoLib::ULong exponent = static_cast<NoLib::ULong>(node.exponent_SLong_Var);
                output.coefficients_RealArray_Var[0] = 1;
                while (exponent > 0)
                {
                    if ((exponent & 1) != 0 && !multiply(output, left, output)) return false;
                    exponent >>= 1;
                    if (exponent != 0 && !multiply(left, left, left)) return false;
                }
                return true;
            }
            Polynomial right;
            if (!toPolynomial(tree, node.right_UInt_Var, right)) return false;
            if (node.type_SymbolicType_Var == NoLib::SymbolicMultiply) return multiply(left, right, output);
            if (node.type_SymbolicType_Var == NoLib::SymbolicDivide)
            {
                if (right.degree_UInt_Var != 0 || right.coefficients_RealArray_Var[0] == 0) return false;
                output = left;
                for (UInt i = 0; i <= output.degree_UInt_Var; ++i)
                {
                    output.coefficients_RealArray_Var[i] /= right.coefficients_RealArray_Var[0];
                    if (!finite(output.coefficients_RealArray_Var[i])) return false;
                }
            }
            else
            {
                output.degree_UInt_Var = NoLib::maximum(left.degree_UInt_Var, right.degree_UInt_Var);
                for (UInt i = 0; i <= output.degree_UInt_Var; ++i)
                {
                    output.coefficients_RealArray_Var[i] = left.coefficients_RealArray_Var[i]
                        + (node.type_SymbolicType_Var == NoLib::SymbolicSubtract ? -1 : 1) * right.coefficients_RealArray_Var[i];
                    if (!finite(output.coefficients_RealArray_Var[i])) return false;
                }
            }
            trimExact(output);
            return true;
        }

        String polynomialText(const NoLib::Polynomial<32>& polynomial)
        {
            String text;
            for (UInt reverse = polynomial.degree_UInt_Var + 1; reverse > 0; --reverse)
            {
                const UInt power = reverse - 1;
                const Real coefficient = polynomial.coefficients_RealArray_Var[power];
                if (coefficient == 0) continue;
                if (!text.empty()) text += coefficient < 0 ? " - " : " + ";
                else if (coefficient < 0) text += '-';
                const Real magnitude = NoLib::absolute(coefficient);
                if (magnitude != 1 || power == 0) text += formatNumber(magnitude);
                if (power > 0)
                {
                    text += 'x';
                    if (power > 1) { text += '^'; text += NoLib::unsignedToString(power); }
                }
            }
            return text.empty() ? String("0") : text;
        }
        bool finitePolynomial(const NoLib::Polynomial<32>& polynomial)
        {
            for (UInt i = 0; i <= polynomial.degree_UInt_Var; ++i)
                if (!finite(polynomial.coefficients_RealArray_Var[i])) return false;
            return true;
        }

        bool verifyIntegral(const ParsedExpression& input, const Expression& integral)
        {
            Expression derivative;
            if (!NoLib::differentiateSymbolic(integral, derivative) || !validConstants(derivative)) return false;
            ParsedExpression parsedDerivative;
            Diagnostic diagnostic;
            if (!parseExpression(formatExpression(derivative), parsedDerivative, diagnostic)) return false;
            // Match the original engine's sample points and tolerance, while
            // evaluating both sides through our range/domain/finite checks.
            const Real samples[] = {-3.25L, -2.0L, -1.25L, -0.5L, -0.125L, 0.0L, 0.25L, 0.75L, 1.5L, 2.25L, 3.5L};
            UInt checked = 0;
            for (Real x : samples)
            {
                Real expected = 0, actual = 0;
                if (!evaluate(input, x, expected, diagnostic) || !evaluate(parsedDerivative, x, actual, diagnostic)) continue;
                const Real scale = NoLib::maximum(1.0L, NoLib::maximum(NoLib::absolute(expected), NoLib::absolute(actual)));
                if (NoLib::absolute(expected - actual) > 2.0e-8L * scale) return false;
                ++checked;
            }
            return checked >= 4;
        }
    }

    String formatNumber(Real value)
    {
        // Avoid the legacy fraction formatter's absolute-zero tolerance and its
        // signed-integer boundary casts. Small coefficients stay visible.
        if (value == 0.0L) return String("0");
        const Real magnitude = NoLib::absolute(value);
        if (magnitude >= 1.0e12L || magnitude < 1.0e-6L) return NoLib::realToString(value, 12);
        return NoLib::fixedRealToString(value, 12);
    }

    String formatExpression(const Expression& expression, const bool* strictLog)
    {
        String text;
        formatNode(expression, expression.root_UInt_Var, 0, 0, strictLog, text);
        return text;
    }

    bool parseCommand(const String& line, Command& command, Diagnostic& error)
    {
        command = Command{};
        error = Diagnostic{};
        auto fail = [&](const char* message, ULong at)
        {
            error.message = message;
            error.position = at;
            return false;
        };
        if (line.length() > LineLimit) return fail("Input exceeds 2048 characters.", LineLimit);
        ULong begin = 0, end = line.length();
        trim(line, begin, end);
        if (begin == end) return fail("Enter a command or type help.", begin);
        bool wrapped = false;
        ULong wordEnd = begin;
        while (wordEnd < end && ((line[wordEnd] >= 'a' && line[wordEnd] <= 'z') || (line[wordEnd] >= 'A' && line[wordEnd] <= 'Z'))) ++wordEnd;
        if (equalText(lowercase(slice(line, begin, wordEnd)), "action"))
        {
            begin = wordEnd;
            while (begin < end && space(line[begin])) ++begin;
            if (begin == end || line[begin] != '[') return fail("Expected '[' after action, for example action[derive(x^3)].", begin);
            if (line[end - 1] != ']') return fail("Expected ']' at the end of action[...].", end);
            ++begin;
            --end;
            trim(line, begin, end);
            wrapped = true;
        }
        ULong open = begin;
        while (open < end && line[open] != '(') ++open;
        ULong nameBegin = begin, nameEnd = open;
        trim(line, nameBegin, nameEnd);
        const String name = lowercase(slice(line, nameBegin, nameEnd));
        if (!commandKind(name, command.kind))
        {
            if (wrapped) return fail("Unknown action. Type help for the command list.", nameBegin);
            // Bare expressions are a convenience: simplify(x^3 + 2x + 1).
            command.kind = CommandKind::Simplify;
            command.arguments[0] = slice(line, begin, end);
            command.offsets[0] = begin;
            command.argumentCount = 1;
            return true;
        }
        if (open < end)
        {
            if (line[end - 1] != ')') return fail("Expected ')' at the end of the command.", end);
            UInt depth = 0;
            ULong start = open + 1;
            for (ULong i = start; i < end - 1; ++i)
            {
                if (line[i] == '(') ++depth;
                else if (line[i] == ')')
                {
                    if (depth == 0) return fail("Unexpected ')' in command.", i);
                    --depth;
                }
                else if (line[i] == '[' || line[i] == ']') return fail("Square brackets belong around the action only.", i);
                if (line[i] == ',' && depth == 0)
                {
                    if (command.argumentCount >= 1) return fail("This command accepts at most two arguments.", i);
                    ULong a = start, b = i;
                    trim(line, a, b);
                    if (a == b) return fail("Missing argument before ','.", i);
                    command.arguments[command.argumentCount] = slice(line, a, b);
                    command.offsets[command.argumentCount++] = a;
                    start = i + 1;
                }
            }
            if (depth != 0) return fail("Unclosed '(' inside command arguments.", end - 1);
            ULong a = start, b = end - 1;
            trim(line, a, b);
            if (a != b)
            {
                command.arguments[command.argumentCount] = slice(line, a, b);
                command.offsets[command.argumentCount++] = a;
            }
            else if (command.argumentCount != 0) return fail("Missing argument after ','.", a);
        }
        const bool math = command.kind <= CommandKind::Evaluate;
        if (math && command.argumentCount == 0) return fail("Put the expression inside parentheses, for example action[dirive(x^3 + 2x + 1)].", open);
        if (command.kind == CommandKind::Evaluate)
        {
            if (command.argumentCount > 2) return fail("Use eval(expression) or eval(expression, x-value).", begin);
        }
        else if ((math || command.kind == CommandKind::Help || command.kind == CommandKind::Theme) && command.argumentCount > 1)
            return fail("This command accepts one argument.", begin);
        else if (!math && command.kind != CommandKind::Help && command.kind != CommandKind::Theme && command.argumentCount > 0)
            return fail("This command does not take arguments.", open + 1);
        return true;
    }

    bool calculate(const Command& command, Result& result, Diagnostic& error)
    {
        result = Result{};
        error = Diagnostic{};
        if (command.kind > CommandKind::Evaluate || command.argumentCount == 0)
        {
            error.message = "This is not a calculation command.";
            return false;
        }
        ParsedExpression parsed;
        if (!parseExpression(command.arguments[0], parsed, error))
        {
            error.position += command.offsets[0];
            return false;
        }
        auto fail = [&](const char* message)
        {
            error.message = message;
            error.position = command.offsets[0];
            return false;
        };
        if (command.kind == CommandKind::Evaluate)
        {
            Real x = 0;
            if (command.argumentCount == 2)
            {
                ParsedExpression point;
                if (!parseExpression(command.arguments[1], point, error)) { error.position += command.offsets[1]; return false; }
                if (!point.constant[point.tree.root_UInt_Var])
                {
                    error.message = "The x-value must be constant, for example 2 or pi/2.";
                    error.position = command.offsets[1];
                    return false;
                }
                x = point.value[point.tree.root_UInt_Var];
            }
            else if (!parsed.constant[parsed.tree.root_UInt_Var]) return fail("This expression contains x. Supply a value: action[eval(x^3 + 2x + 1, 2)].");
            Real value = 0;
            if (!evaluate(parsed, x, value, error)) { error.position += command.offsets[0]; return false; }
            result.label = "VALUE";
            result.text = formatNumber(value);
            if (command.argumentCount == 2) { result.note = "at x = "; result.note += formatNumber(x); }
            return true;
        }
        Expression input;
        if (!prepare(parsed, input) || !validConstants(input)) return fail("Expression exceeds the symbolic engine's capacity or number range.");
        NoLib::Polynomial<32> polynomial;
        const bool isPolynomial = toPolynomial(input, input.root_UInt_Var, polynomial) && finitePolynomial(polynomial);
        if (isPolynomial)
        {
            NoLib::Polynomial<32> output;
            if (command.kind == CommandKind::Derive)
            {
                NoLib::differentiatePolynomial(polynomial, output);
                result.label = "DERIVATIVE / d/dx";
            }
            else if (command.kind == CommandKind::Integrate)
            {
                if (!NoLib::integratePolynomial(polynomial, 0.0L, output)) return fail("Polynomial integration supports input degree up to 31.");
                result.label = "ANTIDERIVATIVE";
            }
            else { output = polynomial; result.label = "SIMPLIFIED"; }
            if (!finitePolynomial(output)) return fail("Polynomial coefficients overflowed the real-number range.");
            result.text = polynomialText(output);
        }
        else if (command.kind == CommandKind::Simplify)
        {
            // Preserve ln semantics and grouping when no polynomial expansion is
            // available; do not falsely claim a general CAS simplification.
            result.label = "PARSED EXPRESSION";
            result.text = formatExpression(parsed.tree, parsed.strictLog);
            result.note = "General expressions are normalized; polynomial terms are expanded and collected up to degree 32.";
        }
        else
        {
            Expression output;
            if (command.kind == CommandKind::Derive)
            {
                if (!NoLib::differentiateSymbolic(input, output)) return fail("Derivative exceeds the symbolic engine's capacity. Try a smaller expression.");
                result.label = "DERIVATIVE / d/dx";
            }
            else
            {
                // Recursive legacy integration allocates an arena per level.
                // Bound its input separately to stay within the Windows stack.
                if (parsed.depth[parsed.tree.root_UInt_Var] > 12 || input.count_UInt_Var > 96)
                    return fail("This integral is too complex for the existing engine. Split it into smaller terms.");
                NoLib::RischIntegrationMethod method = NoLib::RischNoMethod;
                if (!NoLib::rischIntegrateSymbolicNode<NodeLimit, NodeLimit, 16>(input, input.root_UInt_Var, output, output.root_UInt_Var, method))
                    return fail("The existing integrator has no supported rule for this expression.");
                if (!verifyIntegral(parsed, output)) return fail("An antiderivative candidate failed guarded sampled verification; no result was accepted.");
                result.label = "ANTIDERIVATIVE";
                result.note = "Checked at sample points by the existing integrator; this is not a symbolic proof.";
            }
            if (!validConstants(output)) return fail("Symbolic coefficients overflowed the real-number range.");
            result.text = formatExpression(output);
        }
        if (command.kind == CommandKind::Integrate) result.text += " + C";
        if (command.kind == CommandKind::Derive || command.kind == CommandKind::Integrate)
        {
            if (!result.note.empty()) result.note += ' ';
            result.note += "Valid where the original expression is defined; angles are in radians.";
        }
        return true;
    }
}
