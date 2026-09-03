#pragma once

#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Text/String.hpp"

namespace Workbench
{
    constexpr NoLib::UInt NodeLimit = 512;
    constexpr NoLib::UInt DepthLimit = 48;
    constexpr NoLib::ULong LineLimit = 2048;
    using Expression = NoLib::SymbolicExpression<NodeLimit>;

    struct Diagnostic
    {
        NoLib::String message;
        NoLib::ULong position = 0;
    };

    // Sidecar metadata lets the new interface distinguish ln from lnabs without
    // changing the existing library's node representation or public API.
    struct ParsedExpression
    {
        Expression tree;
        bool strictLog[NodeLimit] = {};
        bool constant[NodeLimit] = {};
        NoLib::Real value[NodeLimit] = {};
        NoLib::UInt depth[NodeLimit] = {};
        NoLib::ULong position[NodeLimit] = {};
    };

    bool equalText(const NoLib::String& text, const char* literal);
    NoLib::String slice(const NoLib::String& text, NoLib::ULong begin, NoLib::ULong end);
    bool parseExpression(const NoLib::String& text, ParsedExpression& output, Diagnostic& error);
    bool evaluate(const ParsedExpression& expression, NoLib::Real x, NoLib::Real& result, Diagnostic& error);
    bool finite(NoLib::Real value);
}
