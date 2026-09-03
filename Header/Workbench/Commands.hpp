#pragma once
#include "Workbench/ExpressionParser.hpp"

namespace Workbench
{
    enum class CommandKind { Derive, Integrate, Simplify, Evaluate, Help, Examples, History, Clear, Theme, Quit };

    struct Command
    {
        CommandKind kind = CommandKind::Simplify;
        NoLib::String arguments[2];
        NoLib::ULong offsets[2] = {};
        NoLib::UInt argumentCount = 0;
    };

    struct Result
    {
        NoLib::String label;
        NoLib::String text;
        NoLib::String note;
    };

    bool parseCommand(const NoLib::String& line, Command& command, Diagnostic& error);
    bool calculate(const Command& command, Result& result, Diagnostic& error);
    NoLib::String formatNumber(NoLib::Real value);
    NoLib::String formatExpression(const Expression& expression, const bool* strictLog = nullptr);
}
