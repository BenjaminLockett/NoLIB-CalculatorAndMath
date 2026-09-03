#pragma once

// Precedence-aware human readable output.

#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Text/String.hpp"
#include "NoLib/Text/RealFormatting.hpp"
#include "NoLib/Text/IntegerFormatting.hpp"
#include "NoLib/Core/Utility.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    String symbolicNumberToString(Real value_Real_Var)
    {
        return niceRealToString(value_Real_Var, 1.0e-12L, 64);
    }

    template <UInt Capacity_UInt>
    bool symbolicNodeToString(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, String& result_String_Var)
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
            result_String_Var +=
                symbolicNumberToString<Capacity_UInt>(node_SymbolicNode_Var.constant_Real_Var);
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            result_String_Var += 'x';
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
        {
            if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential)
            {
                result_String_Var += "exp(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute)
            {
                result_String_Var += "lnabs(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine)
            {
                result_String_Var += "sin(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine)
            {
                result_String_Var += "cos(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
            {
                result_String_Var += "atan(";
            }
            else
            {
                result_String_Var += '(';
            }

            if (!symbolicNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var))
            {
                return false;
            }

            result_String_Var += ')';

            if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
            {
                result_String_Var += '^';
                result_String_Var += signedToString(node_SymbolicNode_Var.exponent_SLong_Var);
            }

            return true;
        }

        result_String_Var += '(';

        if (!symbolicNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var))
        {
            return false;
        }

        switch (node_SymbolicNode_Var.type_SymbolicType_Var)
        {
        case SymbolicAdd:
            result_String_Var += " + ";
            break;
        case SymbolicSubtract:
            result_String_Var += " - ";
            break;
        case SymbolicMultiply:
            result_String_Var += " * ";
            break;
        case SymbolicDivide:
            result_String_Var += " / ";
            break;
        default:
            return false;
        }

        if (!symbolicNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, result_String_Var))
        {
            return false;
        }

        result_String_Var += ')';
        return true;
    }

    template <UInt Capacity_UInt>
    UInt symbolicPrecedence(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var)
    {
        SymbolicType type_SymbolicType_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_UInt_Var].type_SymbolicType_Var;

        if (type_SymbolicType_Var == SymbolicAdd || type_SymbolicType_Var == SymbolicSubtract)
        {
            return 1;
        }

        if (type_SymbolicType_Var == SymbolicMultiply || type_SymbolicType_Var == SymbolicDivide)
        {
            return 2;
        }

        if (type_SymbolicType_Var == SymbolicIntegerPower)
        {
            return 3;
        }

        return 4;
    }

    template <UInt Capacity_UInt>
    bool collectSymbolicProduct(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, Real& constant_Real_Var, UInt factors_UIntArray_Var[Capacity_UInt], SLong exponents_SLongArray_Var[Capacity_UInt], UInt& count_UInt_Var)
    {
        const SymbolicNode& node_SymbolicNode_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            return collectSymbolicProduct<Capacity_UInt>(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, constant_Real_Var, factors_UIntArray_Var, exponents_SLongArray_Var, count_UInt_Var) && collectSymbolicProduct<Capacity_UInt>(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, constant_Real_Var, factors_UIntArray_Var, exponents_SLongArray_Var, count_UInt_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            constant_Real_Var *= node_SymbolicNode_Var.constant_Real_Var;
            return true;
        }

        UInt base_UInt_Var = node_UInt_Var;
        SLong exponent_SLong_Var = 1;

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            base_UInt_Var = node_SymbolicNode_Var.left_UInt_Var;
            exponent_SLong_Var = node_SymbolicNode_Var.exponent_SLong_Var;
        }

        for (UInt index_UInt_Var = 0; index_UInt_Var < count_UInt_Var; ++index_UInt_Var)
        {
            if (sameSymbolicNode(expression_Symbolic_Var, factors_UIntArray_Var[index_UInt_Var], expression_Symbolic_Var, base_UInt_Var))
            {
                if ((exponent_SLong_Var > 0 && exponents_SLongArray_Var[index_UInt_Var] > maximumSigned() - exponent_SLong_Var) || (exponent_SLong_Var < 0 && exponents_SLongArray_Var[index_UInt_Var] < -maximumSigned() - 1 - exponent_SLong_Var))
                {
                    break;
                }

                exponents_SLongArray_Var[index_UInt_Var] += exponent_SLong_Var;
                return true;
            }
        }

        if (count_UInt_Var >= Capacity_UInt)
        {
            return false;
        }

        factors_UIntArray_Var[count_UInt_Var] = base_UInt_Var;
        exponents_SLongArray_Var[count_UInt_Var] = exponent_SLong_Var;
        ++count_UInt_Var;
        return true;
    }

    template <UInt Capacity_UInt>
    bool symbolicPrettyNodeToString(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, String& result_String_Var, UInt parentPrecedence_UInt_Var = 0)
    {
        if (node_UInt_Var >= expression_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_UInt_Var];
        UInt precedence_UInt_Var = symbolicPrecedence(expression_Symbolic_Var, node_UInt_Var);
        bool parentheses_Bool_Var = precedence_UInt_Var < parentPrecedence_UInt_Var;

        if (parentheses_Bool_Var)
        {
            result_String_Var += '(';
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            result_String_Var += niceRealToString(node_SymbolicNode_Var.constant_Real_Var, 1.0e-12L, 64);
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            result_String_Var += 'x';
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract)
        {
            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var, precedence_UInt_Var))
            {
                return false;
            }

            result_String_Var += node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd ? " + " : " - ";

            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, result_String_Var, precedence_UInt_Var + (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract ? 1 : 0)))
            {
                return false;
            }
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            Real constant_Real_Var = 1.0L;
            UInt factors_UIntArray_Var[Capacity_UInt];
            SLong exponents_SLongArray_Var[Capacity_UInt];
            UInt count_UInt_Var = 0;

            if (!collectSymbolicProduct<Capacity_UInt>(expression_Symbolic_Var, node_UInt_Var, constant_Real_Var, factors_UIntArray_Var, exponents_SLongArray_Var, count_UInt_Var))
            {
                return false;
            }

            bool printed_Bool_Var = false;

            if (constant_Real_Var != 1.0L || count_UInt_Var == 0)
            {
                result_String_Var += niceRealToString(constant_Real_Var, 1.0e-12L, 64);
                printed_Bool_Var = true;
            }

            for (UInt index_UInt_Var = 0; index_UInt_Var < count_UInt_Var; ++index_UInt_Var)
            {
                if (exponents_SLongArray_Var[index_UInt_Var] == 0)
                {
                    continue;
                }

                SymbolicType factorType_SymbolicType_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[factors_UIntArray_Var[index_UInt_Var]].type_SymbolicType_Var;

                if (printed_Bool_Var && !(index_UInt_Var == 0 && factorType_SymbolicType_Var == SymbolicVariable))
                {
                    result_String_Var += ' ';
                }

                if (!symbolicPrettyNodeToString(expression_Symbolic_Var, factors_UIntArray_Var[index_UInt_Var], result_String_Var, 3))
                {
                    return false;
                }

                if (exponents_SLongArray_Var[index_UInt_Var] != 1)
                {
                    result_String_Var += '^';
                    result_String_Var += signedToString(exponents_SLongArray_Var[index_UInt_Var]);
                }

                printed_Bool_Var = true;
            }
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicDivide)
        {
            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var, precedence_UInt_Var))
            {
                return false;
            }

            result_String_Var += " / ";

            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, result_String_Var, precedence_UInt_Var + 1))
            {
                return false;
            }
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var, precedence_UInt_Var))
            {
                return false;
            }

            result_String_Var += '^';
            result_String_Var += signedToString(node_SymbolicNode_Var.exponent_SLong_Var);
        }
        else
        {
            if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential)
            {
                result_String_Var += "exp(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicLogAbsolute)
            {
                result_String_Var += "lnabs(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine)
            {
                result_String_Var += "sin(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine)
            {
                result_String_Var += "cos(";
            }
            else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicArcTangent)
            {
                result_String_Var += "atan(";
            }
            else
            {
                return false;
            }

            if (!symbolicPrettyNodeToString(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, result_String_Var, 0))
            {
                return false;
            }

            result_String_Var += ')';
        }

        if (parentheses_Bool_Var)
        {
            result_String_Var += ')';
        }

        return true;
    }

    template <UInt Capacity_UInt>
    String symbolicToString(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var)
    {
        String result_String_Var;
        symbolicPrettyNodeToString(expression_Symbolic_Var, expression_Symbolic_Var.root_UInt_Var, result_String_Var);
        return result_String_Var;
    }
}
