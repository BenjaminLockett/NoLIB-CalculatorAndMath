#pragma once

// Symbolic derivative rules.

#include "NoLib/Symbolic/SymbolicCopy.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool differentiateSymbolicNode(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        if (inputNode_UInt_Var >= input_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var =
            input_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[inputNode_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            return makeSymbolicConstant(output_Symbolic_Var, 0.0L, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            return makeSymbolicConstant(output_Symbolic_Var, 1.0L, outputNode_UInt_Var);
        }

        UInt derivativeLeft_UInt_Var = invalidSymbolicNode();

        if (!differentiateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, derivativeLeft_UInt_Var))
        {
            return false;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract)
        {
            UInt derivativeRight_UInt_Var = invalidSymbolicNode();

            if (!differentiateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, derivativeRight_UInt_Var))
            {
                return false;
            }

            return node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd
                ? makeSymbolicAdd(output_Symbolic_Var, derivativeLeft_UInt_Var, derivativeRight_UInt_Var, outputNode_UInt_Var)
                : makeSymbolicSubtract(output_Symbolic_Var, derivativeLeft_UInt_Var, derivativeRight_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            UInt derivativeRight_UInt_Var = invalidSymbolicNode();
            UInt leftCopy_UInt_Var = invalidSymbolicNode();
            UInt rightCopy_UInt_Var = invalidSymbolicNode();
            UInt firstProduct_UInt_Var = invalidSymbolicNode();
            UInt secondProduct_UInt_Var = invalidSymbolicNode();

            return differentiateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, derivativeRight_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, leftCopy_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, rightCopy_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, derivativeLeft_UInt_Var, rightCopy_UInt_Var, firstProduct_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, leftCopy_UInt_Var, derivativeRight_UInt_Var, secondProduct_UInt_Var) &&
                   makeSymbolicAdd(output_Symbolic_Var, firstProduct_UInt_Var, secondProduct_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicDivide)
        {
            UInt derivativeRight_UInt_Var = invalidSymbolicNode();
            UInt leftCopy_UInt_Var = invalidSymbolicNode();
            UInt rightCopyOne_UInt_Var = invalidSymbolicNode();
            UInt rightCopyTwo_UInt_Var = invalidSymbolicNode();
            UInt firstProduct_UInt_Var = invalidSymbolicNode();
            UInt secondProduct_UInt_Var = invalidSymbolicNode();
            UInt numerator_UInt_Var = invalidSymbolicNode();
            UInt denominator_UInt_Var = invalidSymbolicNode();

            return differentiateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, derivativeRight_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, leftCopy_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, rightCopyOne_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, rightCopyTwo_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, derivativeLeft_UInt_Var, rightCopyOne_UInt_Var, firstProduct_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, leftCopy_UInt_Var, derivativeRight_UInt_Var, secondProduct_UInt_Var) &&
                   makeSymbolicSubtract(output_Symbolic_Var, firstProduct_UInt_Var, secondProduct_UInt_Var, numerator_UInt_Var) &&
                   makeSymbolicIntegerPower(output_Symbolic_Var, rightCopyTwo_UInt_Var, 2, denominator_UInt_Var) &&
                   makeSymbolicDivide(output_Symbolic_Var, numerator_UInt_Var, denominator_UInt_Var, outputNode_UInt_Var);
        }

        UInt argumentCopy_UInt_Var = invalidSymbolicNode();

        if (!copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, argumentCopy_UInt_Var))
        {
            return false;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            if (node_SymbolicNode_Var.exponent_SLong_Var == -maximumSigned() - 1)
            {
                return false;
            }

            UInt coefficient_UInt_Var = invalidSymbolicNode();
            UInt reducedPower_UInt_Var = invalidSymbolicNode();
            UInt firstProduct_UInt_Var = invalidSymbolicNode();

            return makeSymbolicConstant(output_Symbolic_Var, static_cast<Real>(node_SymbolicNode_Var.exponent_SLong_Var), coefficient_UInt_Var) &&
                   makeSymbolicIntegerPower(output_Symbolic_Var, argumentCopy_UInt_Var, node_SymbolicNode_Var.exponent_SLong_Var - 1, reducedPower_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, coefficient_UInt_Var, reducedPower_UInt_Var, firstProduct_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, firstProduct_UInt_Var, derivativeLeft_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential)
        {
            UInt exponential_UInt_Var = invalidSymbolicNode();
            return makeSymbolicUnary(output_Symbolic_Var, SymbolicExponential, argumentCopy_UInt_Var, exponential_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, exponential_UInt_Var, derivativeLeft_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute)
        {
            return makeSymbolicDivide(output_Symbolic_Var, derivativeLeft_UInt_Var, argumentCopy_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine)
        {
            UInt cosine_UInt_Var = invalidSymbolicNode();
            return makeSymbolicUnary(output_Symbolic_Var, SymbolicCosine, argumentCopy_UInt_Var, cosine_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, cosine_UInt_Var, derivativeLeft_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine)
        {
            UInt negativeOne_UInt_Var = invalidSymbolicNode();
            UInt sine_UInt_Var = invalidSymbolicNode();
            UInt negativeSine_UInt_Var = invalidSymbolicNode();

            return makeSymbolicConstant(output_Symbolic_Var, -1.0L, negativeOne_UInt_Var) &&
                   makeSymbolicUnary(output_Symbolic_Var, SymbolicSine, argumentCopy_UInt_Var, sine_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, negativeOne_UInt_Var, sine_UInt_Var, negativeSine_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, negativeSine_UInt_Var, derivativeLeft_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
        {
            UInt one_UInt_Var = invalidSymbolicNode();
            UInt squared_UInt_Var = invalidSymbolicNode();
            UInt denominator_UInt_Var = invalidSymbolicNode();

            return makeSymbolicConstant(output_Symbolic_Var, 1.0L, one_UInt_Var) &&
                   makeSymbolicIntegerPower(output_Symbolic_Var, argumentCopy_UInt_Var, 2, squared_UInt_Var) &&
                   makeSymbolicAdd(output_Symbolic_Var, one_UInt_Var, squared_UInt_Var, denominator_UInt_Var) &&
                   makeSymbolicDivide(output_Symbolic_Var, derivativeLeft_UInt_Var, denominator_UInt_Var, outputNode_UInt_Var);
        }

        return false;
    }

    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool differentiateSymbolic(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var)
    {
        output_Symbolic_Var.clear();

        if (!differentiateSymbolicNode(input_Symbolic_Var, input_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, output_Symbolic_Var.root_UInt_Var))
        {
            output_Symbolic_Var.clear();
            return false;
        }

        return true;
    }
}
