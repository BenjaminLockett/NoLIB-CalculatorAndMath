#pragma once

// Elementary rules, substitution and recursive dispatch.

#include "NoLib/Symbolic/SymbolicDifferentiation.hpp"
#include "NoLib/Symbolic/SymbolicSimplification.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool tryRischStyleOuterRule(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt outer_UInt_Var, UInt multiplier_UInt_Var, bool implicitOne_Bool_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        const SymbolicNode& outer_SymbolicNode_Var =
            input_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[outer_UInt_Var];
        SymbolicType outerType_SymbolicType_Var =
            outer_SymbolicNode_Var.type_SymbolicType_Var;

        if (outerType_SymbolicType_Var != SymbolicIntegerPower && outerType_SymbolicType_Var != SymbolicExponential && outerType_SymbolicType_Var != SymbolicLogAbsolute && outerType_SymbolicType_Var != SymbolicSine && outerType_SymbolicType_Var != SymbolicCosine && outerType_SymbolicType_Var != SymbolicArcTangent)
        {
            return false;
        }

        SymbolicExpression<OutputCapacity_UInt> derivative_Symbolic_Var;

        if (!differentiateSymbolicNode(input_Symbolic_Var, outer_SymbolicNode_Var.left_UInt_Var, derivative_Symbolic_Var, derivative_Symbolic_Var.root_UInt_Var))
        {
            return false;
        }

        Real ratio_Real_Var = 0.0L;

        if (!symbolicConstantRatio(input_Symbolic_Var, multiplier_UInt_Var, implicitOne_Bool_Var, derivative_Symbolic_Var, derivative_Symbolic_Var.root_UInt_Var, ratio_Real_Var))
        {
            return false;
        }

        UInt inner_UInt_Var = invalidSymbolicNode();

        if (!copySymbolicNode(input_Symbolic_Var, outer_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, inner_UInt_Var))
        {
            return false;
        }

        if (outerType_SymbolicType_Var == SymbolicIntegerPower)
        {
            SLong exponent_SLong_Var =
                outer_SymbolicNode_Var.exponent_SLong_Var;

            if (exponent_SLong_Var == -1)
            {
                UInt logarithm_UInt_Var = invalidSymbolicNode();
                return makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, inner_UInt_Var, logarithm_UInt_Var) &&
                       scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, logarithm_UInt_Var, outputNode_UInt_Var);
            }

            if (exponent_SLong_Var == maximumSigned())
            {
                return false;
            }

            SLong nextExponent_SLong_Var = exponent_SLong_Var + 1;
            UInt power_UInt_Var = invalidSymbolicNode();
            return makeSymbolicIntegerPower(output_Symbolic_Var, inner_UInt_Var, nextExponent_SLong_Var, power_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var / static_cast<Real>(nextExponent_SLong_Var), power_UInt_Var, outputNode_UInt_Var);
        }

        if (outerType_SymbolicType_Var == SymbolicExponential)
        {
            UInt exponential_UInt_Var = invalidSymbolicNode();
            return makeSymbolicUnary(output_Symbolic_Var, SymbolicExponential, inner_UInt_Var, exponential_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, exponential_UInt_Var, outputNode_UInt_Var);
        }

        if (outerType_SymbolicType_Var == SymbolicCosine)
        {
            UInt sine_UInt_Var = invalidSymbolicNode();
            return makeSymbolicUnary(output_Symbolic_Var, SymbolicSine, inner_UInt_Var, sine_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, sine_UInt_Var, outputNode_UInt_Var);
        }

        if (outerType_SymbolicType_Var == SymbolicSine)
        {
            UInt cosine_UInt_Var = invalidSymbolicNode();
            return makeSymbolicUnary(output_Symbolic_Var, SymbolicCosine, inner_UInt_Var, cosine_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, -ratio_Real_Var, cosine_UInt_Var, outputNode_UInt_Var);
        }

        if (outerType_SymbolicType_Var == SymbolicArcTangent)
        {
            UInt innerForArcTangent_UInt_Var = invalidSymbolicNode();
            UInt innerForSquare_UInt_Var = invalidSymbolicNode();
            UInt arcTangent_UInt_Var = invalidSymbolicNode();
            UInt product_UInt_Var = invalidSymbolicNode();
            UInt squared_UInt_Var = invalidSymbolicNode();
            UInt one_UInt_Var = invalidSymbolicNode();
            UInt logarithmArgument_UInt_Var = invalidSymbolicNode();
            UInt logarithm_UInt_Var = invalidSymbolicNode();
            UInt halfLogarithm_UInt_Var = invalidSymbolicNode();
            UInt difference_UInt_Var = invalidSymbolicNode();

            return copySymbolicNode(input_Symbolic_Var, outer_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, innerForArcTangent_UInt_Var) &&
                   copySymbolicNode(input_Symbolic_Var, outer_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, innerForSquare_UInt_Var) &&
                   makeSymbolicUnary(output_Symbolic_Var, SymbolicArcTangent, innerForArcTangent_UInt_Var, arcTangent_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, inner_UInt_Var, arcTangent_UInt_Var, product_UInt_Var) &&
                   makeSymbolicIntegerPower(output_Symbolic_Var, innerForSquare_UInt_Var, 2, squared_UInt_Var) &&
                   makeSymbolicConstant(output_Symbolic_Var, 1.0L, one_UInt_Var) &&
                   makeSymbolicAdd(output_Symbolic_Var, one_UInt_Var, squared_UInt_Var, logarithmArgument_UInt_Var) &&
                   makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, logarithmArgument_UInt_Var, logarithm_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, 0.5L, logarithm_UInt_Var, halfLogarithm_UInt_Var) &&
                   makeSymbolicSubtract(output_Symbolic_Var, product_UInt_Var, halfLogarithm_UInt_Var, difference_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, difference_UInt_Var, outputNode_UInt_Var);
        }

        UInt innerForProduct_UInt_Var = invalidSymbolicNode();
        UInt logarithm_UInt_Var = invalidSymbolicNode();
        UInt product_UInt_Var = invalidSymbolicNode();
        UInt difference_UInt_Var = invalidSymbolicNode();

        return copySymbolicNode(input_Symbolic_Var, outer_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, innerForProduct_UInt_Var) &&
               makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, inner_UInt_Var, logarithm_UInt_Var) &&
               makeSymbolicMultiply(output_Symbolic_Var, innerForProduct_UInt_Var, logarithm_UInt_Var, product_UInt_Var) &&
               makeSymbolicSubtract(output_Symbolic_Var, product_UInt_Var, inner_UInt_Var, difference_UInt_Var) &&
               scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, difference_UInt_Var, outputNode_UInt_Var);
    }

    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool rischStyleIntegrateSymbolicNode(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
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
            UInt constant_UInt_Var = invalidSymbolicNode();
            UInt variable_UInt_Var = invalidSymbolicNode();
            return makeSymbolicConstant(output_Symbolic_Var, node_SymbolicNode_Var.constant_Real_Var, constant_UInt_Var) &&
                   makeSymbolicVariable(output_Symbolic_Var, variable_UInt_Var) &&
                   makeSymbolicMultiply(output_Symbolic_Var, constant_UInt_Var, variable_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            UInt variable_UInt_Var = invalidSymbolicNode();
            UInt squared_UInt_Var = invalidSymbolicNode();
            return makeSymbolicVariable(output_Symbolic_Var, variable_UInt_Var) &&
                   makeSymbolicIntegerPower(output_Symbolic_Var, variable_UInt_Var, 2, squared_UInt_Var) &&
                   scaleSymbolicNode(output_Symbolic_Var, 0.5L, squared_UInt_Var, outputNode_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract)
        {
            UInt left_UInt_Var = invalidSymbolicNode();
            UInt right_UInt_Var = invalidSymbolicNode();

            if (!rischStyleIntegrateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, left_UInt_Var) || !rischStyleIntegrateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, right_UInt_Var))
            {
                return false;
            }

            return node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd
                ? makeSymbolicAdd(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var)
                : makeSymbolicSubtract(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);
        }

        if (tryRischStyleOuterRule(input_Symbolic_Var, inputNode_UInt_Var, invalidSymbolicNode(), true, output_Symbolic_Var, outputNode_UInt_Var))
        {
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            if (tryRischStyleOuterRule(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, node_SymbolicNode_Var.right_UInt_Var, false, output_Symbolic_Var, outputNode_UInt_Var) || tryRischStyleOuterRule(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, node_SymbolicNode_Var.left_UInt_Var, false, output_Symbolic_Var, outputNode_UInt_Var))
            {
                return true;
            }

            Real constant_Real_Var = 0.0L;
            UInt other_UInt_Var = invalidSymbolicNode();

            if (symbolicConstantValue(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, constant_Real_Var))
            {
                other_UInt_Var = node_SymbolicNode_Var.right_UInt_Var;
            }
            else if (symbolicConstantValue(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, constant_Real_Var))
            {
                other_UInt_Var = node_SymbolicNode_Var.left_UInt_Var;
            }

            if (other_UInt_Var != invalidSymbolicNode())
            {
                UInt integral_UInt_Var = invalidSymbolicNode();
                return rischStyleIntegrateSymbolicNode(input_Symbolic_Var, other_UInt_Var, output_Symbolic_Var, integral_UInt_Var) &&
                       scaleSymbolicNode(output_Symbolic_Var, constant_Real_Var, integral_UInt_Var, outputNode_UInt_Var);
            }
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicDivide)
        {
            SymbolicExpression<OutputCapacity_UInt> derivative_Symbolic_Var;

            if (!differentiateSymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, derivative_Symbolic_Var, derivative_Symbolic_Var.root_UInt_Var))
            {
                return false;
            }

            Real ratio_Real_Var = 0.0L;

            if (symbolicConstantRatio(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, false, derivative_Symbolic_Var, derivative_Symbolic_Var.root_UInt_Var, ratio_Real_Var))
            {
                UInt denominator_UInt_Var = invalidSymbolicNode();
                UInt logarithm_UInt_Var = invalidSymbolicNode();
                return copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, denominator_UInt_Var) &&
                       makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, denominator_UInt_Var, logarithm_UInt_Var) &&
                       scaleSymbolicNode(output_Symbolic_Var, ratio_Real_Var, logarithm_UInt_Var, outputNode_UInt_Var);
            }
        }

        return false;
    }

    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt>
    bool rischStyleIntegrateSymbolic(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var)
    {
        output_Symbolic_Var.clear();

        if (!rischStyleIntegrateSymbolicNode(input_Symbolic_Var, input_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, output_Symbolic_Var.root_UInt_Var))
        {
            output_Symbolic_Var.clear();
            return false;
        }

        return true;
    }
}
