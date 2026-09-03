#pragma once

// Rational reduction by polynomial division.

#include "NoLib/Symbolic/IntegrationCommon.hpp"
#include "NoLib/Symbolic/PolynomialBridge.hpp"
#include "NoLib/Symbolic/IntegrationMethod.hpp"
#include "NoLib/Scalar/SquareRoot.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt, UInt MaximumDegree_UInt>
    bool tryRischRationalReduction(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var)
    {
        if (inputNode_UInt_Var >= input_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[inputNode_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var != SymbolicDivide)
        {
            return false;
        }

        Polynomial<MaximumDegree_UInt> numerator_Polynomial_Var;
        Polynomial<MaximumDegree_UInt> denominator_Polynomial_Var;

        if (!symbolicNodeToPolynomial(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, numerator_Polynomial_Var) || !symbolicNodeToPolynomial(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, denominator_Polynomial_Var))
        {
            return false;
        }

        trimPolynomial(denominator_Polynomial_Var);

        if (denominator_Polynomial_Var.degree_UInt_Var == 0 || denominator_Polynomial_Var.degree_UInt_Var > 2)
        {
            return false;
        }

        Polynomial<MaximumDegree_UInt> quotient_Polynomial_Var;
        Polynomial<MaximumDegree_UInt> remainder_Polynomial_Var;

        if (!dividePolynomials(numerator_Polynomial_Var, denominator_Polynomial_Var, quotient_Polynomial_Var, remainder_Polynomial_Var))
        {
            return false;
        }

        UInt result_UInt_Var = invalidSymbolicNode();

        if (!integratedPolynomialToSymbolicNode(quotient_Polynomial_Var, output_Symbolic_Var, result_UInt_Var))
        {
            return false;
        }

        if (remainder_Polynomial_Var.degree_UInt_Var == 0 && absolute(remainder_Polynomial_Var.coefficients_RealArray_Var[0]) <= 1.0e-14L)
        {
            outputNode_UInt_Var = result_UInt_Var;
            return true;
        }

        UInt remainderIntegral_UInt_Var = invalidSymbolicNode();

        if (denominator_Polynomial_Var.degree_UInt_Var == 1)
        {
            Real slope_Real_Var = denominator_Polynomial_Var.coefficients_RealArray_Var[1];

            if (slope_Real_Var == 0.0L || remainder_Polynomial_Var.degree_UInt_Var != 0)
            {
                return false;
            }

            UInt denominator_UInt_Var = invalidSymbolicNode();
            UInt logarithm_UInt_Var = invalidSymbolicNode();

            if (!copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, denominator_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, denominator_UInt_Var, logarithm_UInt_Var) || !scaleSymbolicNode(output_Symbolic_Var, remainder_Polynomial_Var.coefficients_RealArray_Var[0] / slope_Real_Var, logarithm_UInt_Var, remainderIntegral_UInt_Var))
            {
                return false;
            }
        }
        else
        {
            Real a_Real_Var = denominator_Polynomial_Var.coefficients_RealArray_Var[2];
            Real b_Real_Var = denominator_Polynomial_Var.coefficients_RealArray_Var[1];
            Real c_Real_Var = denominator_Polynomial_Var.coefficients_RealArray_Var[0];
            Real m_Real_Var = remainder_Polynomial_Var.degree_UInt_Var >= 1 ? remainder_Polynomial_Var.coefficients_RealArray_Var[1] : 0.0L;
            Real n_Real_Var = remainder_Polynomial_Var.coefficients_RealArray_Var[0];

            if (a_Real_Var == 0.0L)
            {
                return false;
            }

            Real logarithmScale_Real_Var = m_Real_Var / (2.0L * a_Real_Var);
            Real residualScale_Real_Var = n_Real_Var - logarithmScale_Real_Var * b_Real_Var;
            UInt denominator_UInt_Var = invalidSymbolicNode();
            UInt logarithm_UInt_Var = invalidSymbolicNode();
            UInt logarithmTerm_UInt_Var = invalidSymbolicNode();

            if (!copySymbolicNode(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, denominator_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, denominator_UInt_Var, logarithm_UInt_Var) || !scaleSymbolicNode(output_Symbolic_Var, absolute(logarithmScale_Real_Var), logarithm_UInt_Var, logarithmTerm_UInt_Var))
            {
                return false;
            }

            if (absolute(residualScale_Real_Var) <= 1.0e-14L)
            {
                if (logarithmScale_Real_Var < 0.0L)
                {
                    if (!scaleSymbolicNode(output_Symbolic_Var, -1.0L, logarithmTerm_UInt_Var, remainderIntegral_UInt_Var))
                    {
                        return false;
                    }
                }
                else
                {
                    remainderIntegral_UInt_Var = logarithmTerm_UInt_Var;
                }
            }
            else
            {
                Polynomial<MaximumDegree_UInt> linear_Polynomial_Var;
                linear_Polynomial_Var.degree_UInt_Var = 1;
                linear_Polynomial_Var.coefficients_RealArray_Var[0] = b_Real_Var;
                linear_Polynomial_Var.coefficients_RealArray_Var[1] = 2.0L * a_Real_Var;
                Real positiveDiscriminant_Real_Var = 4.0L * a_Real_Var * c_Real_Var - b_Real_Var * b_Real_Var;
                Real discriminantScale_Real_Var = maximum(1.0L, maximum(absolute(4.0L * a_Real_Var * c_Real_Var), absolute(b_Real_Var * b_Real_Var)));
                Real discriminantTolerance_Real_Var = 1.0e-13L * discriminantScale_Real_Var;
                UInt basicIntegral_UInt_Var = invalidSymbolicNode();

                if (positiveDiscriminant_Real_Var > discriminantTolerance_Real_Var)
                {
                    Real root_Real_Var = 0.0L;
                    UInt normalizedLinear_UInt_Var = invalidSymbolicNode();
                    UInt arcTangent_UInt_Var = invalidSymbolicNode();

                    if (!squareRoot(positiveDiscriminant_Real_Var, root_Real_Var))
                    {
                        return false;
                    }

                    linear_Polynomial_Var.coefficients_RealArray_Var[0] /= root_Real_Var;
                    linear_Polynomial_Var.coefficients_RealArray_Var[1] /= root_Real_Var;

                    if (!polynomialToSymbolicNode(linear_Polynomial_Var, output_Symbolic_Var, normalizedLinear_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicArcTangent, normalizedLinear_UInt_Var, arcTangent_UInt_Var) || !scaleSymbolicNode(output_Symbolic_Var, 2.0L / root_Real_Var, arcTangent_UInt_Var, basicIntegral_UInt_Var))
                    {
                        return false;
                    }
                }
                else if (positiveDiscriminant_Real_Var < -discriminantTolerance_Real_Var)
                {
                    Real root_Real_Var = 0.0L;
                    UInt linearOne_UInt_Var = invalidSymbolicNode();
                    UInt linearTwo_UInt_Var = invalidSymbolicNode();
                    UInt one_UInt_Var = invalidSymbolicNode();
                    UInt secondOne_UInt_Var = invalidSymbolicNode();
                    UInt numerator_UInt_Var = invalidSymbolicNode();
                    UInt denominatorPart_UInt_Var = invalidSymbolicNode();
                    UInt ratio_UInt_Var = invalidSymbolicNode();
                    UInt logarithmRatio_UInt_Var = invalidSymbolicNode();

                    if (!squareRoot(-positiveDiscriminant_Real_Var, root_Real_Var))
                    {
                        return false;
                    }

                    linear_Polynomial_Var.coefficients_RealArray_Var[0] /= root_Real_Var;
                    linear_Polynomial_Var.coefficients_RealArray_Var[1] /= root_Real_Var;

                    if (!polynomialToSymbolicNode(linear_Polynomial_Var, output_Symbolic_Var, linearOne_UInt_Var) || !polynomialToSymbolicNode(linear_Polynomial_Var, output_Symbolic_Var, linearTwo_UInt_Var) || !makeSymbolicConstant(output_Symbolic_Var, 1.0L, one_UInt_Var) || !makeSymbolicConstant(output_Symbolic_Var, 1.0L, secondOne_UInt_Var) || !makeSymbolicSubtract(output_Symbolic_Var, linearOne_UInt_Var, one_UInt_Var, numerator_UInt_Var) || !makeSymbolicAdd(output_Symbolic_Var, linearTwo_UInt_Var, secondOne_UInt_Var, denominatorPart_UInt_Var) || !makeSymbolicDivide(output_Symbolic_Var, numerator_UInt_Var, denominatorPart_UInt_Var, ratio_UInt_Var) || !makeSymbolicUnary(output_Symbolic_Var, SymbolicLogAbsolute, ratio_UInt_Var, logarithmRatio_UInt_Var) || !scaleSymbolicNode(output_Symbolic_Var, 1.0L / root_Real_Var, logarithmRatio_UInt_Var, basicIntegral_UInt_Var))
                    {
                        return false;
                    }
                }
                else
                {
                    UInt linear_UInt_Var = invalidSymbolicNode();
                    UInt negativeOne_UInt_Var = invalidSymbolicNode();

                    linear_Polynomial_Var.coefficients_RealArray_Var[0] *= 0.5L;
                    linear_Polynomial_Var.coefficients_RealArray_Var[1] *= 0.5L;

                    if (!polynomialToSymbolicNode(linear_Polynomial_Var, output_Symbolic_Var, linear_UInt_Var) || !makeSymbolicConstant(output_Symbolic_Var, -1.0L, negativeOne_UInt_Var) || !makeSymbolicDivide(output_Symbolic_Var, negativeOne_UInt_Var, linear_UInt_Var, basicIntegral_UInt_Var))
                    {
                        return false;
                    }
                }

                UInt residualTerm_UInt_Var = invalidSymbolicNode();

                if (!scaleSymbolicNode(output_Symbolic_Var, residualScale_Real_Var, basicIntegral_UInt_Var, residualTerm_UInt_Var))
                {
                    return false;
                }

                if (logarithmScale_Real_Var < 0.0L)
                {
                    if (!makeSymbolicSubtract(output_Symbolic_Var, residualTerm_UInt_Var, logarithmTerm_UInt_Var, remainderIntegral_UInt_Var))
                    {
                        return false;
                    }
                }
                else if (!makeSymbolicAdd(output_Symbolic_Var, logarithmTerm_UInt_Var, residualTerm_UInt_Var, remainderIntegral_UInt_Var))
                {
                    return false;
                }
            }
        }

        return makeSymbolicAdd(output_Symbolic_Var, result_UInt_Var, remainderIntegral_UInt_Var, outputNode_UInt_Var);
    }
}
