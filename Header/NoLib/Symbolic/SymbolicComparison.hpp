#pragma once

// Structural comparison between two arenas.

#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Core/Utility.hpp"

namespace NoLib
{
    template <UInt LeftCapacity_UInt, UInt RightCapacity_UInt>
    bool sameSymbolicNode(const SymbolicExpression<LeftCapacity_UInt>& left_Symbolic_Var, UInt leftNode_UInt_Var, const SymbolicExpression<RightCapacity_UInt>& right_Symbolic_Var, UInt rightNode_UInt_Var)
    {
        if (leftNode_UInt_Var >= left_Symbolic_Var.count_UInt_Var || rightNode_UInt_Var >= right_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& left_SymbolicNode_Var =
            left_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[leftNode_UInt_Var];
        const SymbolicNode& right_SymbolicNode_Var =
            right_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[rightNode_UInt_Var];

        if (left_SymbolicNode_Var.type_SymbolicType_Var != right_SymbolicNode_Var.type_SymbolicType_Var)
        {
            return false;
        }

        if (left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            Real scale_Real_Var = maximum(1.0L, maximum(absolute(left_SymbolicNode_Var.constant_Real_Var), absolute(right_SymbolicNode_Var.constant_Real_Var)));
            return absolute(left_SymbolicNode_Var.constant_Real_Var - right_SymbolicNode_Var.constant_Real_Var) <=
                   1.0e-14L * scale_Real_Var;
        }

        if (left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            return true;
        }

        if (left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower && left_SymbolicNode_Var.exponent_SLong_Var != right_SymbolicNode_Var.exponent_SLong_Var)
        {
            return false;
        }

        bool leftMatches_Bool_Var = sameSymbolicNode(left_Symbolic_Var, left_SymbolicNode_Var.left_UInt_Var, right_Symbolic_Var, right_SymbolicNode_Var.left_UInt_Var);

        if (left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower || left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential || left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute || left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine || left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine || left_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
        {
            return leftMatches_Bool_Var;
        }

        bool rightMatches_Bool_Var = sameSymbolicNode(left_Symbolic_Var, left_SymbolicNode_Var.right_UInt_Var, right_Symbolic_Var, right_SymbolicNode_Var.right_UInt_Var);

        if (leftMatches_Bool_Var && rightMatches_Bool_Var)
        {
            return true;
        }

        if (left_SymbolicNode_Var.type_SymbolicType_Var != SymbolicAdd && left_SymbolicNode_Var.type_SymbolicType_Var != SymbolicMultiply)
        {
            return false;
        }

        return sameSymbolicNode(left_Symbolic_Var, left_SymbolicNode_Var.left_UInt_Var, right_Symbolic_Var, right_SymbolicNode_Var.right_UInt_Var) &&
               sameSymbolicNode(left_Symbolic_Var, left_SymbolicNode_Var.right_UInt_Var, right_Symbolic_Var, right_SymbolicNode_Var.left_UInt_Var);
    }
}
