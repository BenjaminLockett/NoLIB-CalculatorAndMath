#pragma once

// One node of a symbolic expression arena.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Symbolic/SymbolicType.hpp"

namespace NoLib
{
    UInt invalidSymbolicNode();

    struct SymbolicNode
    {
        SymbolicType type_SymbolicType_Var;
        Real constant_Real_Var;
        SLong exponent_SLong_Var;
        UInt left_UInt_Var;
        UInt right_UInt_Var;
    };
}
