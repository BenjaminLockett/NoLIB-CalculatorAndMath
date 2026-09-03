#pragma once

// Polynomial-exponential and polynomial-trigonometric solvers.

#include "NoLib/Symbolic/IntegrationRational.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt, UInt MaximumDegree_UInt>
    bool tryRischPolynomialElementaryExtension(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var, RischIntegrationMethod& method_RischIntegrationMethod_Var)
    {
        if (inputNode_UInt_Var >= input_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[inputNode_UInt_Var];
        UInt unary_UInt_Var = invalidSymbolicNode();
        UInt polynomial_UInt_Var = invalidSymbolicNode();

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicCosine)
        {
            unary_UInt_Var = inputNode_UInt_Var;
        }
        else if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            SymbolicType leftType_SymbolicType_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_SymbolicNode_Var.left_UInt_Var].type_SymbolicType_Var;
            SymbolicType rightType_SymbolicType_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[node_SymbolicNode_Var.right_UInt_Var].type_SymbolicType_Var;

            if (leftType_SymbolicType_Var == SymbolicExponential || leftType_SymbolicType_Var == SymbolicSine || leftType_SymbolicType_Var == SymbolicCosine)
            {
                unary_UInt_Var = node_SymbolicNode_Var.left_UInt_Var;
                polynomial_UInt_Var = node_SymbolicNode_Var.right_UInt_Var;
            }
            else if (rightType_SymbolicType_Var == SymbolicExponential || rightType_SymbolicType_Var == SymbolicSine || rightType_SymbolicType_Var == SymbolicCosine)
            {
                unary_UInt_Var = node_SymbolicNode_Var.right_UInt_Var;
                polynomial_UInt_Var = node_SymbolicNode_Var.left_UInt_Var;
            }
        }

        if (unary_UInt_Var == invalidSymbolicNode())
        {
            return false;
        }

        const SymbolicNode& unary_SymbolicNode_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[unary_UInt_Var];
        Polynomial<MaximumDegree_UInt> argument_Polynomial_Var;
        Polynomial<MaximumDegree_UInt> multiplier_Polynomial_Var;

        if (!symbolicNodeToPolynomial(input_Symbolic_Var, unary_SymbolicNode_Var.left_UInt_Var, argument_Polynomial_Var) || argument_Polynomial_Var.degree_UInt_Var != 1 || argument_Polynomial_Var.coefficients_RealArray_Var[1] == 0.0L)
        {
            return false;
        }

        if (polynomial_UInt_Var == invalidSymbolicNode())
        {
            multiplier_Polynomial_Var.coefficients_RealArray_Var[0] = 1.0L;
        }
        else if (!symbolicNodeToPolynomial(input_Symbolic_Var, polynomial_UInt_Var, multiplier_Polynomial_Var))
        {
            return false;
        }

        Real slope_Real_Var = argument_Polynomial_Var.coefficients_RealArray_Var[1];

        if (unary_SymbolicNode_Var.type_SymbolicType_Var == SymbolicExponential)
        {
            Polynomial<MaximumDegree_UInt> solution_Polynomial_Var;
            solution_Polynomial_Var.degree_UInt_Var = multiplier_Polynomial_Var.degree_UInt_Var;

            for (UInt reverse_UInt_Var = multiplier_Polynomial_Var.degree_UInt_Var + 1; reverse_UInt_Var > 0; --reverse_UInt_Var)
            {
                UInt index_UInt_Var = reverse_UInt_Var - 1;
                Real derivativeContribution_Real_Var = index_UInt_Var < multiplier_Polynomial_Var.degree_UInt_Var ? static_cast<Real>(index_UInt_Var + 1) * solution_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var + 1] : 0.0L;
                solution_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = (multiplier_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] - derivativeContribution_Real_Var) / slope_Real_Var;
            }

            UInt solution_UInt_Var = invalidSymbolicNode();
            UInt argument_UInt_Var = invalidSymbolicNode();
            UInt exponential_UInt_Var = invalidSymbolicNode();

            if (!polynomialToSymbolicNode(solution_Polynomial_Var, output_Symbolic_Var, solution_UInt_Var) || !copySymbolicNode(input_Symbolic_Var, unary_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, argument_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicExponential, argument_UInt_Var, exponential_UInt_Var) || !makeSymbolicMultiply(output_Symbolic_Var, solution_UInt_Var, exponential_UInt_Var, outputNode_UInt_Var))
            {
                return false;
            }

            method_RischIntegrationMethod_Var = RischPolynomialExponential;
            return true;
        }

        Polynomial<MaximumDegree_UInt> sineCoefficient_Polynomial_Var;
        Polynomial<MaximumDegree_UInt> cosineCoefficient_Polynomial_Var;
        sineCoefficient_Polynomial_Var.degree_UInt_Var = multiplier_Polynomial_Var.degree_UInt_Var;
        cosineCoefficient_Polynomial_Var.degree_UInt_Var = multiplier_Polynomial_Var.degree_UInt_Var;

        for (UInt reverse_UInt_Var = multiplier_Polynomial_Var.degree_UInt_Var + 1; reverse_UInt_Var > 0; --reverse_UInt_Var)
        {
            UInt index_UInt_Var = reverse_UInt_Var - 1;
            Real nextSine_Real_Var = index_UInt_Var < multiplier_Polynomial_Var.degree_UInt_Var ? sineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var + 1] : 0.0L;
            Real nextCosine_Real_Var = index_UInt_Var < multiplier_Polynomial_Var.degree_UInt_Var ? cosineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var + 1] : 0.0L;
            Real multiplier_Real_Var = multiplier_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var];

            if (unary_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSine)
            {
                sineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = -static_cast<Real>(index_UInt_Var + 1) * nextCosine_Real_Var / slope_Real_Var;
                cosineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = (static_cast<Real>(index_UInt_Var + 1) * nextSine_Real_Var - multiplier_Real_Var) / slope_Real_Var;
            }
            else
            {
                sineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = (multiplier_Real_Var - static_cast<Real>(index_UInt_Var + 1) * nextCosine_Real_Var) / slope_Real_Var;
                cosineCoefficient_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = static_cast<Real>(index_UInt_Var + 1) * nextSine_Real_Var / slope_Real_Var;
            }
        }

        trimPolynomial(sineCoefficient_Polynomial_Var);
        trimPolynomial(cosineCoefficient_Polynomial_Var);
        UInt sineCoefficient_UInt_Var = invalidSymbolicNode();
        UInt cosineCoefficient_UInt_Var = invalidSymbolicNode();
        UInt argumentOne_UInt_Var = invalidSymbolicNode();
        UInt argumentTwo_UInt_Var = invalidSymbolicNode();
        UInt sine_UInt_Var = invalidSymbolicNode();
        UInt cosine_UInt_Var = invalidSymbolicNode();
        UInt sineTerm_UInt_Var = invalidSymbolicNode();
        UInt cosineTerm_UInt_Var = invalidSymbolicNode();

        if (!polynomialToSymbolicNode(sineCoefficient_Polynomial_Var, output_Symbolic_Var, sineCoefficient_UInt_Var) || !polynomialToSymbolicNode(cosineCoefficient_Polynomial_Var, output_Symbolic_Var, cosineCoefficient_UInt_Var) || !copySymbolicNode(input_Symbolic_Var, unary_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, argumentOne_UInt_Var) || !copySymbolicNode(input_Symbolic_Var, unary_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, argumentTwo_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicSine, argumentOne_UInt_Var, sine_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicCosine, argumentTwo_UInt_Var, cosine_UInt_Var) || !makeSymbolicMultiply(output_Symbolic_Var, sineCoefficient_UInt_Var, sine_UInt_Var, sineTerm_UInt_Var) || !makeSymbolicMultiply(output_Symbolic_Var, cosineCoefficient_UInt_Var, cosine_UInt_Var, cosineTerm_UInt_Var) || !makeSymbolicAdd(output_Symbolic_Var, sineTerm_UInt_Var, cosineTerm_UInt_Var, outputNode_UInt_Var))
        {
            return false;
        }

        method_RischIntegrationMethod_Var = RischPolynomialTrigonometric;
        return true;
    }
}
