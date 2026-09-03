#pragma once

// Depth-bounded real evaluation with domain checks.

#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Scalar/Exponential.hpp"
#include "NoLib/Scalar/Logarithm.hpp"
#include "NoLib/Scalar/Trigonometry.hpp"
#include "NoLib/Scalar/Powers.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Core/RealTraits.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    bool evaluateSymbolicNode(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, Real x_Real_Var, Real& result_Real_Var)
    {
        if (node_UInt_Var >= expression_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var =
            expression_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[node_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            result_Real_Var = node_SymbolicNode_Var.constant_Real_Var;
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            result_Real_Var = x_Real_Var;
            return true;
        }

        Real left_Real_Var = 0.0L;

        if (!evaluateSymbolicNode(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, x_Real_Var, left_Real_Var))
        {
            return false;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            return realIntegerPower(left_Real_Var, node_SymbolicNode_Var.exponent_SLong_Var, result_Real_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential)
        {
            return exponential(left_Real_Var, result_Real_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute)
        {
            return naturalLog(absolute(left_Real_Var), result_Real_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine)
        {
            result_Real_Var = sine(left_Real_Var);
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine)
        {
            result_Real_Var = cosine(left_Real_Var);
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
        {
            result_Real_Var = arcTangent(left_Real_Var);
            return true;
        }

        Real right_Real_Var = 0.0L;

        if (!evaluateSymbolicNode(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, x_Real_Var, right_Real_Var))
        {
            return false;
        }

        switch (node_SymbolicNode_Var.type_SymbolicType_Var)
        {
        case SymbolicAdd:
            result_Real_Var = left_Real_Var + right_Real_Var;
            return true;
        case SymbolicSubtract:
            result_Real_Var = left_Real_Var - right_Real_Var;
            return true;
        case SymbolicMultiply:
            return safeMultiplyReal(left_Real_Var, right_Real_Var, result_Real_Var);
        case SymbolicDivide:
            if (right_Real_Var == 0.0L)
            {
                return false;
            }

            result_Real_Var = left_Real_Var / right_Real_Var;
            return true;
        default:
            return false;
        }
    }

    template <UInt Capacity_UInt>
    bool evaluateSymbolic(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, Real x_Real_Var, Real& result_Real_Var)
    {
        return evaluateSymbolicNode(expression_Symbolic_Var, expression_Symbolic_Var.root_UInt_Var, x_Real_Var, result_Real_Var);
    }
}
