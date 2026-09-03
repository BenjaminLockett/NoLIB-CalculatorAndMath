#pragma once

// Node operation and function tags.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    enum SymbolicType
    {
        SymbolicConstant,
        SymbolicVariable,
        SymbolicAdd,
        SymbolicSubtract,
        SymbolicMultiply,
        SymbolicDivide,
        SymbolicIntegerPower,
        SymbolicExponential,
        SymbolicLogAbsolute,
        SymbolicSine,
        SymbolicCosine,
        SymbolicArcTangent
    };
}
