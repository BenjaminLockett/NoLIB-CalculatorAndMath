#pragma once

// Depth-bounded copying between arenas.

#include "NoLib/Symbolic/SymbolicBuilder.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool copySymbolicNode(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        if (inputNode_UInt_Var >= input_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& inputNode_SymbolicNode_Var =
            input_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[inputNode_UInt_Var];

        if (inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            return makeSymbolicConstant(output_Symbolic_Var, inputNode_SymbolicNode_Var.constant_Real_Var, outputNode_UInt_Var);
        }

        if (inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            return makeSymbolicVariable(output_Symbolic_Var, outputNode_UInt_Var);
        }

        UInt left_UInt_Var = invalidSymbolicNode();

        if (!copySymbolicNode(input_Symbolic_Var, inputNode_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, left_UInt_Var))
        {
            return false;
        }

        if (inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            return makeSymbolicIntegerPower(output_Symbolic_Var, left_UInt_Var, inputNode_SymbolicNode_Var.exponent_SLong_Var, outputNode_UInt_Var);
        }

        if (inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential || inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute || inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine || inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine || inputNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
        {
            return makeSymbolicUnary(output_Symbolic_Var, inputNode_SymbolicNode_Var.type_SymbolicType_Var, left_UInt_Var, outputNode_UInt_Var);
        }

        UInt right_UInt_Var = invalidSymbolicNode();

        if (!copySymbolicNode(input_Symbolic_Var, inputNode_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, right_UInt_Var))
        {
            return false;
        }

        switch (inputNode_SymbolicNode_Var.type_SymbolicType_Var)
        {
        case SymbolicAdd:
            return makeSymbolicAdd(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);
        case SymbolicSubtract:
            return makeSymbolicSubtract(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);
        case SymbolicMultiply:
            return makeSymbolicMultiply(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);
        case SymbolicDivide:
            return makeSymbolicDivide(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);
        default:
            return false;
        }
    }
}
