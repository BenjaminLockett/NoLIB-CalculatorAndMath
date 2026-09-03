#pragma once

// Durand-Kerner root finding with residual checks.

#include "NoLib/Polynomial/Polynomial.hpp"
#include "NoLib/Polynomial/PolynomialCalculus.hpp"
#include "NoLib/Complex/ComplexArithmetic.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Scalar/Constants.hpp"
#include "NoLib/Scalar/Trigonometry.hpp"

namespace NoLib
{
    template <UInt MaximumDegree_UInt>
    bool findPolynomialRoots(const Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, ComplexNumber (&roots_ComplexArray_Var)[MaximumDegree_UInt], UInt& usedIterations_UInt_Var, UInt maximumIterations_UInt_Var = 2000, Real tolerance_Real_Var = 1.0e-14L)
    {
        UInt degree_UInt_Var = polynomial_Polynomial_Var.degree_UInt_Var;

        if (degree_UInt_Var == 0 || degree_UInt_Var > MaximumDegree_UInt)
        {
            return false;
        }

        Real leading_Real_Var =
            polynomial_Polynomial_Var
                .coefficients_RealArray_Var[degree_UInt_Var];

        if (absolute(leading_Real_Var) <= tolerance_Real_Var)
        {
            return false;
        }

        if (degree_UInt_Var == 1)
        {
            roots_ComplexArray_Var[0] = makeComplex(-polynomial_Polynomial_Var.coefficients_RealArray_Var[0] / leading_Real_Var);
            usedIterations_UInt_Var = 0;
            return true;
        }

        Real radius_Real_Var = 1.0L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < degree_UInt_Var; ++index_UInt_Var)
        {
            radius_Real_Var = maximum(radius_Real_Var, 1.0L + absolute(polynomial_Polynomial_Var .coefficients_RealArray_Var[index_UInt_Var] / leading_Real_Var));
        }

        Real pi_Real_Var = piApproximation();

        for (UInt index_UInt_Var = 0; index_UInt_Var < degree_UInt_Var; ++index_UInt_Var)
        {
            Real angle_Real_Var =
                2.0L * pi_Real_Var *
                static_cast<Real>(index_UInt_Var) /
                static_cast<Real>(degree_UInt_Var);
            roots_ComplexArray_Var[index_UInt_Var] = makeComplex(radius_Real_Var * cosine(angle_Real_Var), radius_Real_Var * sine(angle_Real_Var));
        }

        ComplexNumber nextRoots_ComplexArray_Var[MaximumDegree_UInt];

        for (usedIterations_UInt_Var = 0; usedIterations_UInt_Var < maximumIterations_UInt_Var; ++usedIterations_UInt_Var)
        {
            Real largestChange_Real_Var = 0.0L;

            for (UInt rootIndex_UInt_Var = 0; rootIndex_UInt_Var < degree_UInt_Var; ++rootIndex_UInt_Var)
            {
                ComplexNumber denominator_Complex_Var =
                    makeComplex(1.0L);

                for (UInt otherIndex_UInt_Var = 0; otherIndex_UInt_Var < degree_UInt_Var; ++otherIndex_UInt_Var)
                {
                    if (otherIndex_UInt_Var != rootIndex_UInt_Var)
                    {
                        denominator_Complex_Var = multiplyComplex(denominator_Complex_Var, subtractComplex(roots_ComplexArray_Var[rootIndex_UInt_Var], roots_ComplexArray_Var[otherIndex_UInt_Var]));
                    }
                }

                denominator_Complex_Var = multiplyComplex(denominator_Complex_Var, makeComplex(leading_Real_Var));

                if (complexMagnitude(denominator_Complex_Var) <= tolerance_Real_Var * tolerance_Real_Var)
                {
                    denominator_Complex_Var = addComplex(denominator_Complex_Var, makeComplex(tolerance_Real_Var, tolerance_Real_Var));
                }

                ComplexNumber correction_Complex_Var;

                if (!divideComplex(evaluatePolynomial(polynomial_Polynomial_Var, roots_ComplexArray_Var[rootIndex_UInt_Var]), denominator_Complex_Var, correction_Complex_Var))
                {
                    return false;
                }

                nextRoots_ComplexArray_Var[rootIndex_UInt_Var] =
                    subtractComplex(roots_ComplexArray_Var[rootIndex_UInt_Var], correction_Complex_Var);
                largestChange_Real_Var = maximum(largestChange_Real_Var, complexMagnitude(correction_Complex_Var));
            }

            for (UInt index_UInt_Var = 0; index_UInt_Var < degree_UInt_Var; ++index_UInt_Var)
            {
                roots_ComplexArray_Var[index_UInt_Var] =
                    nextRoots_ComplexArray_Var[index_UInt_Var];
            }

            if (largestChange_Real_Var <= tolerance_Real_Var)
            {
                ++usedIterations_UInt_Var;
                return true;
            }
        }

        return false;
    }
}
