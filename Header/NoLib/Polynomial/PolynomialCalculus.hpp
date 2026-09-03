#pragma once

// Evaluation, differentiation and integration.

#include "NoLib/Polynomial/Polynomial.hpp"
#include "NoLib/Complex/ComplexNumber.hpp"
#include "NoLib/Complex/ComplexArithmetic.hpp"

namespace NoLib
{
    template <UInt MaximumDegree_UInt>
    Real evaluatePolynomial(const Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, Real x_Real_Var)
    {
        Real result_Real_Var =
            polynomial_Polynomial_Var
                .coefficients_RealArray_Var[
                    polynomial_Polynomial_Var.degree_UInt_Var];

        for (UInt index_UInt_Var = polynomial_Polynomial_Var.degree_UInt_Var; index_UInt_Var > 0; --index_UInt_Var)
        {
            result_Real_Var =
                result_Real_Var * x_Real_Var +
                polynomial_Polynomial_Var
                    .coefficients_RealArray_Var[index_UInt_Var - 1];
        }

        return result_Real_Var;
    }

    template <UInt MaximumDegree_UInt>
    ComplexNumber evaluatePolynomial(const Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, const ComplexNumber& x_Complex_Var)
    {
        ComplexNumber result_Complex_Var = makeComplex(polynomial_Polynomial_Var .coefficients_RealArray_Var[ polynomial_Polynomial_Var.degree_UInt_Var]);

        for (UInt index_UInt_Var = polynomial_Polynomial_Var.degree_UInt_Var; index_UInt_Var > 0; --index_UInt_Var)
        {
            result_Complex_Var = addComplex(multiplyComplex(result_Complex_Var, x_Complex_Var), makeComplex(polynomial_Polynomial_Var .coefficients_RealArray_Var[index_UInt_Var - 1]));
        }

        return result_Complex_Var;
    }

    template <UInt MaximumDegree_UInt>
    void differentiatePolynomial(const Polynomial<MaximumDegree_UInt>& input_Polynomial_Var, Polynomial<MaximumDegree_UInt>& output_Polynomial_Var)
    {
        output_Polynomial_Var = Polynomial<MaximumDegree_UInt>();

        if (input_Polynomial_Var.degree_UInt_Var == 0)
        {
            return;
        }

        output_Polynomial_Var.degree_UInt_Var =
            input_Polynomial_Var.degree_UInt_Var - 1;

        for (UInt index_UInt_Var = 1; index_UInt_Var <= input_Polynomial_Var.degree_UInt_Var; ++index_UInt_Var)
        {
            output_Polynomial_Var
                .coefficients_RealArray_Var[index_UInt_Var - 1] =
                input_Polynomial_Var
                    .coefficients_RealArray_Var[index_UInt_Var] *
                static_cast<Real>(index_UInt_Var);
        }
    }

    template <UInt MaximumDegree_UInt>
    bool integratePolynomial(const Polynomial<MaximumDegree_UInt>& input_Polynomial_Var, Real integrationConstant_Real_Var, Polynomial<MaximumDegree_UInt>& output_Polynomial_Var)
    {
        if (input_Polynomial_Var.degree_UInt_Var >= MaximumDegree_UInt)
        {
            return false;
        }

        output_Polynomial_Var = Polynomial<MaximumDegree_UInt>();
        output_Polynomial_Var.degree_UInt_Var =
            input_Polynomial_Var.degree_UInt_Var + 1;
        output_Polynomial_Var.coefficients_RealArray_Var[0] =
            integrationConstant_Real_Var;

        for (UInt index_UInt_Var = 0; index_UInt_Var <= input_Polynomial_Var.degree_UInt_Var; ++index_UInt_Var)
        {
            output_Polynomial_Var
                .coefficients_RealArray_Var[index_UInt_Var + 1] =
                input_Polynomial_Var
                    .coefficients_RealArray_Var[index_UInt_Var] /
                static_cast<Real>(index_UInt_Var + 1);
        }

        return true;
    }
}
