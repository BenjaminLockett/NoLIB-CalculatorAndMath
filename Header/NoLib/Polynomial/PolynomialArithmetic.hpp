#pragma once

// Trim, add, multiply, power and divide.

#include "NoLib/Polynomial/Polynomial.hpp"
#include "NoLib/Core/Utility.hpp"

namespace NoLib
{
    template <UInt MaximumDegree_UInt>
    void trimPolynomial(Polynomial<MaximumDegree_UInt>& polynomial_Polynomial_Var, Real tolerance_Real_Var = 1.0e-14L)
    {
        while (polynomial_Polynomial_Var.degree_UInt_Var > 0 && absolute(polynomial_Polynomial_Var.coefficients_RealArray_Var[polynomial_Polynomial_Var.degree_UInt_Var]) <= tolerance_Real_Var)
        {
            polynomial_Polynomial_Var.coefficients_RealArray_Var[polynomial_Polynomial_Var.degree_UInt_Var] = 0.0L;
            --polynomial_Polynomial_Var.degree_UInt_Var;
        }
    }

    template <UInt MaximumDegree_UInt>
    bool addPolynomials(const Polynomial<MaximumDegree_UInt>& left_Polynomial_Var, const Polynomial<MaximumDegree_UInt>& right_Polynomial_Var, bool subtract_Bool_Var, Polynomial<MaximumDegree_UInt>& result_Polynomial_Var)
    {
        result_Polynomial_Var = Polynomial<MaximumDegree_UInt>();
        result_Polynomial_Var.degree_UInt_Var = maximum(left_Polynomial_Var.degree_UInt_Var, right_Polynomial_Var.degree_UInt_Var);

        for (UInt index_UInt_Var = 0; index_UInt_Var <= result_Polynomial_Var.degree_UInt_Var; ++index_UInt_Var)
        {
            Real left_Real_Var = index_UInt_Var <= left_Polynomial_Var.degree_UInt_Var ? left_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] : 0.0L;
            Real right_Real_Var = index_UInt_Var <= right_Polynomial_Var.degree_UInt_Var ? right_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] : 0.0L;
            result_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var] = subtract_Bool_Var ? left_Real_Var - right_Real_Var : left_Real_Var + right_Real_Var;
        }

        trimPolynomial(result_Polynomial_Var);
        return true;
    }

    template <UInt MaximumDegree_UInt>
    bool multiplyPolynomials(const Polynomial<MaximumDegree_UInt>& left_Polynomial_Var, const Polynomial<MaximumDegree_UInt>& right_Polynomial_Var, Polynomial<MaximumDegree_UInt>& result_Polynomial_Var)
    {
        if (left_Polynomial_Var.degree_UInt_Var > MaximumDegree_UInt - right_Polynomial_Var.degree_UInt_Var)
        {
            return false;
        }

        result_Polynomial_Var = Polynomial<MaximumDegree_UInt>();
        result_Polynomial_Var.degree_UInt_Var = left_Polynomial_Var.degree_UInt_Var + right_Polynomial_Var.degree_UInt_Var;

        for (UInt leftIndex_UInt_Var = 0; leftIndex_UInt_Var <= left_Polynomial_Var.degree_UInt_Var; ++leftIndex_UInt_Var)
        {
            for (UInt rightIndex_UInt_Var = 0; rightIndex_UInt_Var <= right_Polynomial_Var.degree_UInt_Var; ++rightIndex_UInt_Var)
            {
                result_Polynomial_Var.coefficients_RealArray_Var[leftIndex_UInt_Var + rightIndex_UInt_Var] += left_Polynomial_Var.coefficients_RealArray_Var[leftIndex_UInt_Var] * right_Polynomial_Var.coefficients_RealArray_Var[rightIndex_UInt_Var];
            }
        }

        trimPolynomial(result_Polynomial_Var);
        return true;
    }

    template <UInt MaximumDegree_UInt>
    bool powerPolynomial(const Polynomial<MaximumDegree_UInt>& base_Polynomial_Var, ULong exponent_ULong_Var, Polynomial<MaximumDegree_UInt>& result_Polynomial_Var)
    {
        Polynomial<MaximumDegree_UInt> resultWorking_Polynomial_Var;
        resultWorking_Polynomial_Var.coefficients_RealArray_Var[0] = 1.0L;
        Polynomial<MaximumDegree_UInt> factor_Polynomial_Var = base_Polynomial_Var;

        while (exponent_ULong_Var > 0)
        {
            if ((exponent_ULong_Var & 1ULL) != 0)
            {
                Polynomial<MaximumDegree_UInt> next_Polynomial_Var;

                if (!multiplyPolynomials(resultWorking_Polynomial_Var, factor_Polynomial_Var, next_Polynomial_Var))
                {
                    return false;
                }

                resultWorking_Polynomial_Var = next_Polynomial_Var;
            }

            exponent_ULong_Var >>= 1;

            if (exponent_ULong_Var > 0)
            {
                Polynomial<MaximumDegree_UInt> next_Polynomial_Var;

                if (!multiplyPolynomials(factor_Polynomial_Var, factor_Polynomial_Var, next_Polynomial_Var))
                {
                    return false;
                }

                factor_Polynomial_Var = next_Polynomial_Var;
            }
        }

        result_Polynomial_Var = resultWorking_Polynomial_Var;
        return true;
    }

    template <UInt MaximumDegree_UInt>
    bool dividePolynomials(const Polynomial<MaximumDegree_UInt>& numerator_Polynomial_Var, const Polynomial<MaximumDegree_UInt>& denominator_Polynomial_Var, Polynomial<MaximumDegree_UInt>& quotient_Polynomial_Var, Polynomial<MaximumDegree_UInt>& remainder_Polynomial_Var)
    {
        Real denominatorLeading_Real_Var = denominator_Polynomial_Var.coefficients_RealArray_Var[denominator_Polynomial_Var.degree_UInt_Var];

        if (denominatorLeading_Real_Var == 0.0L)
        {
            return false;
        }

        quotient_Polynomial_Var = Polynomial<MaximumDegree_UInt>();
        remainder_Polynomial_Var = numerator_Polynomial_Var;
        trimPolynomial(remainder_Polynomial_Var);

        while (remainder_Polynomial_Var.degree_UInt_Var >= denominator_Polynomial_Var.degree_UInt_Var && !(remainder_Polynomial_Var.degree_UInt_Var == 0 && absolute(remainder_Polynomial_Var.coefficients_RealArray_Var[0]) <= 1.0e-14L))
        {
            UInt degreeDifference_UInt_Var = remainder_Polynomial_Var.degree_UInt_Var - denominator_Polynomial_Var.degree_UInt_Var;
            Real factor_Real_Var = remainder_Polynomial_Var.coefficients_RealArray_Var[remainder_Polynomial_Var.degree_UInt_Var] / denominatorLeading_Real_Var;
            quotient_Polynomial_Var.coefficients_RealArray_Var[degreeDifference_UInt_Var] += factor_Real_Var;
            quotient_Polynomial_Var.degree_UInt_Var = maximum(quotient_Polynomial_Var.degree_UInt_Var, degreeDifference_UInt_Var);

            for (UInt index_UInt_Var = 0; index_UInt_Var <= denominator_Polynomial_Var.degree_UInt_Var; ++index_UInt_Var)
            {
                remainder_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var + degreeDifference_UInt_Var] -= factor_Real_Var * denominator_Polynomial_Var.coefficients_RealArray_Var[index_UInt_Var];
            }

            trimPolynomial(remainder_Polynomial_Var, 1.0e-12L);
        }

        trimPolynomial(quotient_Polynomial_Var);
        return true;
    }
}
