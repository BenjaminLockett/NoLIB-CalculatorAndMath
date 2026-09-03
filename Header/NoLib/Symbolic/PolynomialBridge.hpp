#pragma once

// Symbolic to polynomial conversion in both directions.

#include "NoLib/Symbolic/SymbolicBuilder.hpp"
#include "NoLib/Polynomial/PolynomialArithmetic.hpp"

namespace NoLib
{
    template <UInt SymbolicCapacity_UInt, UInt MaximumDegree_UInt>
    bool symbolicNodeToPolynomial(const SymbolicExpression<SymbolicCapacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, Polynomial<MaximumDegree_UInt>& result_Polynomial_Var)
    {
        if (node_UInt_Var >= expression_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var = expression_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_UInt_Var];
        result_Polynomial_Var = Polynomial<MaximumDegree_UInt>();

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicConstant)
        {
            result_Polynomial_Var.coefficients_RealArray_Var[0] = node_SymbolicNode_Var.constant_Real_Var;
            return true;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicVariable)
        {
            if (MaximumDegree_UInt < 1)
            {
                return false;
            }

            result_Polynomial_Var.degree_UInt_Var = 1;
            result_Polynomial_Var.coefficients_RealArray_Var[1] = 1.0L;
            return true;
        }

        Polynomial<MaximumDegree_UInt> left_Polynomial_Var;

        if (!symbolicNodeToPolynomial(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, left_Polynomial_Var))
        {
            return false;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicIntegerPower)
        {
            if (node_SymbolicNode_Var.exponent_SLong_Var < 0)
            {
                return false;
            }

            return powerPolynomial(left_Polynomial_Var, static_cast<ULong>(node_SymbolicNode_Var.exponent_SLong_Var), result_Polynomial_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var != SymbolicAdd && node_SymbolicNode_Var.type_SymbolicType_Var != SymbolicSubtract && node_SymbolicNode_Var.type_SymbolicType_Var != SymbolicMultiply && node_SymbolicNode_Var.type_SymbolicType_Var != SymbolicDivide)
        {
            return false;
        }

        Polynomial<MaximumDegree_UInt> right_Polynomial_Var;

        if (!symbolicNodeToPolynomial(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, right_Polynomial_Var))
        {
            return false;
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd)
        {
            return addPolynomials(left_Polynomial_Var, right_Polynomial_Var, false, result_Polynomial_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract)
        {
            return addPolynomials(left_Polynomial_Var, right_Polynomial_Var, true, result_Polynomial_Var);
        }

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            return multiplyPolynomials(left_Polynomial_Var, right_Polynomial_Var, result_Polynomial_Var);
        }

        if (right_Polynomial_Var.degree_UInt_Var != 0 || right_Polynomial_Var.coefficients_RealArray_Var[0] == 0.0L)
        {
            return false;
        }

        result_Polynomial_Var = left_Polynomial_Var;

        for (UInt index_UInt_Var = 0; index_UInt_Var <= result_Polynomial_Var.degree_UInt_Var; ++index_UInt_Var)
        {
            result_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] /= right_Polynomial_Var.coefficients_RealArray_Var[0];
        }

        return true;
    }

    template <UInt SymbolicCapacity_UInt, UInt MaximumDegree_UInt>
    bool symbolicToPolynomial(const SymbolicExpression<SymbolicCapacity_UInt>& expression_Symbolic_Var, Polynomial<MaximumDegree_UInt>& result_Polynomial_Var)
    {
        if (!symbolicNodeToPolynomial(expression_Symbolic_Var, expression_Symbolic_Var.root_UInt_Var, result_Polynomial_Var))
        {
            return false;
        }

        trimPolynomial(result_Polynomial_Var);
        return true;
    }

    template <UInt MaximumDegree_UInt, UInt Capacity_UInt>
    bool polynomialToSymbolicNode(const Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, SymbolicExpression<Capacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        bool hasTerm_Bool_Var = false;
        UInt result_UInt_Var = invalidSymbolicNode();

        for (UInt reverse_UInt_Var = polynomial_Polynomial_Var.degree_UInt_Var + 1; reverse_UInt_Var > 0; --reverse_UInt_Var)
        {
            UInt power_UInt_Var = reverse_UInt_Var - 1;
            Real coefficient_Real_Var = polynomial_Polynomial_Var.coefficients_RealArray_Var[power_UInt_Var];

            if (absolute(coefficient_Real_Var) <= 1.0e-14L)
            {
                continue;
            }

            Real termCoefficient_Real_Var = hasTerm_Bool_Var && coefficient_Real_Var < 0.0L ? -coefficient_Real_Var : coefficient_Real_Var;
            UInt term_UInt_Var = invalidSymbolicNode();

            if (power_UInt_Var == 0)
            {
                if (!makeSymbolicConstant(output_Symbolic_Var, termCoefficient_Real_Var, term_UInt_Var))
                {
                    return false;
                }
            }
            else
            {
                UInt variable_UInt_Var = invalidSymbolicNode();
                UInt powerNode_UInt_Var = invalidSymbolicNode();

                if (!makeSymbolicVariable(output_Symbolic_Var, variable_UInt_Var) || !makeSymbolicIntegerPower(output_Symbolic_Var, variable_UInt_Var, static_cast<SLong>(power_UInt_Var), powerNode_UInt_Var))
                {
                    return false;
                }

                if (termCoefficient_Real_Var == 1.0L)
                {
                    term_UInt_Var = powerNode_UInt_Var;
                }
                else
                {
                    UInt coefficient_UInt_Var = invalidSymbolicNode();

                    if (!makeSymbolicConstant(output_Symbolic_Var, termCoefficient_Real_Var, coefficient_UInt_Var) || !makeSymbolicMultiply(output_Symbolic_Var, coefficient_UInt_Var, powerNode_UInt_Var, term_UInt_Var))
                    {
                        return false;
                    }
                }
            }

            if (!hasTerm_Bool_Var)
            {
                result_UInt_Var = term_UInt_Var;
                hasTerm_Bool_Var = true;
            }
            else if (coefficient_Real_Var < 0.0L ? !makeSymbolicSubtract(output_Symbolic_Var, result_UInt_Var, term_UInt_Var, result_UInt_Var) : !makeSymbolicAdd(output_Symbolic_Var, result_UInt_Var, term_UInt_Var, result_UInt_Var))
            {
                return false;
            }
        }

        if (!hasTerm_Bool_Var)
        {
            return makeSymbolicConstant(output_Symbolic_Var, 0.0L, outputNode_UInt_Var);
        }

        outputNode_UInt_Var = result_UInt_Var;
        return true;
    }

    template <UInt MaximumDegree_UInt, UInt Capacity_UInt>
    bool integratedPolynomialToSymbolicNode(const Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, SymbolicExpression<Capacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        Polynomial<MaximumDegree_UInt> integrated_Polynomial_Var;
        return integratePolynomial(polynomial_Polynomial_Var, 0.0L, integrated_Polynomial_Var) && polynomialToSymbolicNode(integrated_Polynomial_Var, output_Symbolic_Var, outputNode_UInt_Var);
    }
}
