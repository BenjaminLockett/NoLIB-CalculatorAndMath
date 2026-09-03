#pragma once

// Validated and locally simplified node construction.

#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Core/RealTraits.hpp"
#include "NoLib/Scalar/Powers.hpp"
#include "NoLib/Core/CheckedInteger.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    bool makeSymbolicConstant(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, Real value_Real_Var, UInt& result_UInt_Var)
    {
        SymbolicNode node_SymbolicNode_Var;
        node_SymbolicNode_Var.type_SymbolicType_Var = SymbolicConstant;
        node_SymbolicNode_Var.constant_Real_Var = value_Real_Var;
        node_SymbolicNode_Var.exponent_SLong_Var = 0;
        node_SymbolicNode_Var.left_UInt_Var = invalidSymbolicNode();
        node_SymbolicNode_Var.right_UInt_Var = invalidSymbolicNode();
        return expression_Symbolic_Var.addNode(node_SymbolicNode_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicVariable(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt& result_UInt_Var)
    {
        SymbolicNode node_SymbolicNode_Var;
        node_SymbolicNode_Var.type_SymbolicType_Var = SymbolicVariable;
        node_SymbolicNode_Var.constant_Real_Var = 0.0L;
        node_SymbolicNode_Var.exponent_SLong_Var = 0;
        node_SymbolicNode_Var.left_UInt_Var = invalidSymbolicNode();
        node_SymbolicNode_Var.right_UInt_Var = invalidSymbolicNode();
        return expression_Symbolic_Var.addNode(node_SymbolicNode_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool symbolicConstantValue(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, Real& value_Real_Var)
    {
        if (node_UInt_Var >= expression_Symbolic_Var.count_UInt_Var || expression_Symbolic_Var .nodes_SymbolicNodeArray_Var[node_UInt_Var] .type_SymbolicType_Var != SymbolicConstant)
        {
            return false;
        }

        value_Real_Var = expression_Symbolic_Var
            .nodes_SymbolicNodeArray_Var[node_UInt_Var]
            .constant_Real_Var;
        return true;
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicBinaryRaw(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, SymbolicType type_SymbolicType_Var, UInt left_UInt_Var, UInt right_UInt_Var, UInt& result_UInt_Var)
    {
        SymbolicNode node_SymbolicNode_Var;
        node_SymbolicNode_Var.type_SymbolicType_Var =
            type_SymbolicType_Var;
        node_SymbolicNode_Var.constant_Real_Var = 0.0L;
        node_SymbolicNode_Var.exponent_SLong_Var = 0;
        node_SymbolicNode_Var.left_UInt_Var = left_UInt_Var;
        node_SymbolicNode_Var.right_UInt_Var = right_UInt_Var;
        return expression_Symbolic_Var.addNode(node_SymbolicNode_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicAdd(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt left_UInt_Var, UInt right_UInt_Var, UInt& result_UInt_Var)
    {
        Real left_Real_Var = 0.0L;
        Real right_Real_Var = 0.0L;
        bool leftConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, left_UInt_Var, left_Real_Var);
        bool rightConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, right_UInt_Var, right_Real_Var);

        if (leftConstant_Bool_Var && rightConstant_Bool_Var)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, left_Real_Var + right_Real_Var, result_UInt_Var);
        }

        if (leftConstant_Bool_Var && left_Real_Var == 0.0L)
        {
            result_UInt_Var = right_UInt_Var;
            return true;
        }

        if (rightConstant_Bool_Var && right_Real_Var == 0.0L)
        {
            result_UInt_Var = left_UInt_Var;
            return true;
        }

        if (rightConstant_Bool_Var && right_Real_Var < 0.0L)
        {
            UInt positive_UInt_Var = invalidSymbolicNode();
            return makeSymbolicConstant(expression_Symbolic_Var, -right_Real_Var, positive_UInt_Var) && makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicSubtract, left_UInt_Var, positive_UInt_Var, result_UInt_Var);
        }

        if (right_UInt_Var < expression_Symbolic_Var.count_UInt_Var)
        {
            const SymbolicNode& rightNode_SymbolicNode_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[right_UInt_Var];

            if (rightNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
            {
                Real factor_Real_Var = 0.0L;
                UInt core_UInt_Var = invalidSymbolicNode();

                if (symbolicConstantValue(expression_Symbolic_Var, rightNode_SymbolicNode_Var.left_UInt_Var, factor_Real_Var) && factor_Real_Var < 0.0L)
                {
                    core_UInt_Var = rightNode_SymbolicNode_Var.right_UInt_Var;
                }
                else if (symbolicConstantValue(expression_Symbolic_Var, rightNode_SymbolicNode_Var.right_UInt_Var, factor_Real_Var) && factor_Real_Var < 0.0L)
                {
                    core_UInt_Var = rightNode_SymbolicNode_Var.left_UInt_Var;
                }

                if (core_UInt_Var != invalidSymbolicNode())
                {
                    UInt positiveFactor_UInt_Var = invalidSymbolicNode();
                    UInt positiveTerm_UInt_Var = invalidSymbolicNode();
                    return makeSymbolicConstant(expression_Symbolic_Var, -factor_Real_Var, positiveFactor_UInt_Var) && makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicMultiply, positiveFactor_UInt_Var, core_UInt_Var, positiveTerm_UInt_Var) && makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicSubtract, left_UInt_Var, positiveTerm_UInt_Var, result_UInt_Var);
                }
            }
        }

        return makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicAdd, left_UInt_Var, right_UInt_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicSubtract(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt left_UInt_Var, UInt right_UInt_Var, UInt& result_UInt_Var)
    {
        Real left_Real_Var = 0.0L;
        Real right_Real_Var = 0.0L;
        bool leftConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, left_UInt_Var, left_Real_Var);
        bool rightConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, right_UInt_Var, right_Real_Var);

        if (leftConstant_Bool_Var && rightConstant_Bool_Var)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, left_Real_Var - right_Real_Var, result_UInt_Var);
        }

        if (rightConstant_Bool_Var && right_Real_Var == 0.0L)
        {
            result_UInt_Var = left_UInt_Var;
            return true;
        }

        return makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicSubtract, left_UInt_Var, right_UInt_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicMultiply(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt left_UInt_Var, UInt right_UInt_Var, UInt& result_UInt_Var)
    {
        Real left_Real_Var = 0.0L;
        Real right_Real_Var = 0.0L;
        bool leftConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, left_UInt_Var, left_Real_Var);
        bool rightConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, right_UInt_Var, right_Real_Var);

        if ((leftConstant_Bool_Var && left_Real_Var == 0.0L) || (rightConstant_Bool_Var && right_Real_Var == 0.0L))
        {
            return makeSymbolicConstant(expression_Symbolic_Var, 0.0L, result_UInt_Var);
        }

        if (leftConstant_Bool_Var && rightConstant_Bool_Var)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, left_Real_Var * right_Real_Var, result_UInt_Var);
        }

        if (leftConstant_Bool_Var && left_Real_Var == 1.0L)
        {
            result_UInt_Var = right_UInt_Var;
            return true;
        }

        if (rightConstant_Bool_Var && right_Real_Var == 1.0L)
        {
            result_UInt_Var = left_UInt_Var;
            return true;
        }

        if (leftConstant_Bool_Var)
        {
            const SymbolicNode& rightNode_SymbolicNode_Var =
                expression_Symbolic_Var
                    .nodes_SymbolicNodeArray_Var[right_UInt_Var];

            if (rightNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
            {
                Real nestedConstant_Real_Var = 0.0L;
                UInt nestedOther_UInt_Var = invalidSymbolicNode();

                if (symbolicConstantValue(expression_Symbolic_Var, rightNode_SymbolicNode_Var.left_UInt_Var, nestedConstant_Real_Var))
                {
                    nestedOther_UInt_Var =
                        rightNode_SymbolicNode_Var.right_UInt_Var;
                }
                else if (symbolicConstantValue(expression_Symbolic_Var, rightNode_SymbolicNode_Var.right_UInt_Var, nestedConstant_Real_Var))
                {
                    nestedOther_UInt_Var =
                        rightNode_SymbolicNode_Var.left_UInt_Var;
                }

                if (nestedOther_UInt_Var != invalidSymbolicNode())
                {
                    UInt combinedConstant_UInt_Var = invalidSymbolicNode();
                    return makeSymbolicConstant(expression_Symbolic_Var, left_Real_Var * nestedConstant_Real_Var, combinedConstant_UInt_Var) &&
                           makeSymbolicMultiply(expression_Symbolic_Var, combinedConstant_UInt_Var, nestedOther_UInt_Var, result_UInt_Var);
                }
            }
        }

        if (rightConstant_Bool_Var)
        {
            const SymbolicNode& leftNode_SymbolicNode_Var =
                expression_Symbolic_Var
                    .nodes_SymbolicNodeArray_Var[left_UInt_Var];

            if (leftNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
            {
                Real nestedConstant_Real_Var = 0.0L;
                UInt nestedOther_UInt_Var = invalidSymbolicNode();

                if (symbolicConstantValue(expression_Symbolic_Var, leftNode_SymbolicNode_Var.left_UInt_Var, nestedConstant_Real_Var))
                {
                    nestedOther_UInt_Var =
                        leftNode_SymbolicNode_Var.right_UInt_Var;
                }
                else if (symbolicConstantValue(expression_Symbolic_Var, leftNode_SymbolicNode_Var.right_UInt_Var, nestedConstant_Real_Var))
                {
                    nestedOther_UInt_Var =
                        leftNode_SymbolicNode_Var.left_UInt_Var;
                }

                if (nestedOther_UInt_Var != invalidSymbolicNode())
                {
                    UInt combinedConstant_UInt_Var = invalidSymbolicNode();
                    return makeSymbolicConstant(expression_Symbolic_Var, right_Real_Var * nestedConstant_Real_Var, combinedConstant_UInt_Var) &&
                           makeSymbolicMultiply(expression_Symbolic_Var, combinedConstant_UInt_Var, nestedOther_UInt_Var, result_UInt_Var);
                }
            }
        }

        return makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicMultiply, left_UInt_Var, right_UInt_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicDivide(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt numerator_UInt_Var, UInt denominator_UInt_Var, UInt& result_UInt_Var)
    {
        Real numerator_Real_Var = 0.0L;
        Real denominator_Real_Var = 0.0L;
        bool numeratorConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, numerator_UInt_Var, numerator_Real_Var);
        bool denominatorConstant_Bool_Var = symbolicConstantValue(expression_Symbolic_Var, denominator_UInt_Var, denominator_Real_Var);

        if (denominatorConstant_Bool_Var && denominator_Real_Var == 0.0L)
        {
            return false;
        }

        if (numeratorConstant_Bool_Var && numerator_Real_Var == 0.0L)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, 0.0L, result_UInt_Var);
        }

        if (numeratorConstant_Bool_Var && denominatorConstant_Bool_Var)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, numerator_Real_Var / denominator_Real_Var, result_UInt_Var);
        }

        if (denominatorConstant_Bool_Var && denominator_Real_Var == 1.0L)
        {
            result_UInt_Var = numerator_UInt_Var;
            return true;
        }

        return makeSymbolicBinaryRaw(expression_Symbolic_Var, SymbolicDivide, numerator_UInt_Var, denominator_UInt_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicIntegerPower(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt base_UInt_Var, SLong exponent_SLong_Var, UInt& result_UInt_Var)
    {
        if (exponent_SLong_Var == 0)
        {
            return makeSymbolicConstant(expression_Symbolic_Var, 1.0L, result_UInt_Var);
        }

        if (exponent_SLong_Var == 1)
        {
            result_UInt_Var = base_UInt_Var;
            return true;
        }

        Real base_Real_Var = 0.0L;

        if (symbolicConstantValue(expression_Symbolic_Var, base_UInt_Var, base_Real_Var))
        {
            Real result_Real_Var = 0.0L;

            if (!realIntegerPower(base_Real_Var, exponent_SLong_Var, result_Real_Var))
            {
                return false;
            }

            return makeSymbolicConstant(expression_Symbolic_Var, result_Real_Var, result_UInt_Var);
        }

        if (base_UInt_Var < expression_Symbolic_Var.count_UInt_Var)
        {
            const SymbolicNode& baseNode_SymbolicNode_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[base_UInt_Var];

            if (baseNode_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
            {
                SLong combinedExponent_SLong_Var = 0;

                if (multiplySignedChecked(baseNode_SymbolicNode_Var.exponent_SLong_Var, exponent_SLong_Var, combinedExponent_SLong_Var))
                {
                    return makeSymbolicIntegerPower(expression_Symbolic_Var, baseNode_SymbolicNode_Var.left_UInt_Var, combinedExponent_SLong_Var, result_UInt_Var);
                }
            }
        }

        SymbolicNode node_SymbolicNode_Var;
        node_SymbolicNode_Var.type_SymbolicType_Var =
            SymbolicIntegerPower;
        node_SymbolicNode_Var.constant_Real_Var = 0.0L;
        node_SymbolicNode_Var.exponent_SLong_Var = exponent_SLong_Var;
        node_SymbolicNode_Var.left_UInt_Var = base_UInt_Var;
        node_SymbolicNode_Var.right_UInt_Var = invalidSymbolicNode();
        return expression_Symbolic_Var.addNode(node_SymbolicNode_Var, result_UInt_Var);
    }

    template <UInt Capacity_UInt>
    bool makeSymbolicUnary(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, SymbolicType type_SymbolicType_Var, UInt argument_UInt_Var, UInt& result_UInt_Var)
    {
        SymbolicNode node_SymbolicNode_Var;
        node_SymbolicNode_Var.type_SymbolicType_Var =
            type_SymbolicType_Var;
        node_SymbolicNode_Var.constant_Real_Var = 0.0L;
        node_SymbolicNode_Var.exponent_SLong_Var = 0;
        node_SymbolicNode_Var.left_UInt_Var = argument_UInt_Var;
        node_SymbolicNode_Var.right_UInt_Var = invalidSymbolicNode();
        return expression_Symbolic_Var.addNode(node_SymbolicNode_Var, result_UInt_Var);
    }
}
