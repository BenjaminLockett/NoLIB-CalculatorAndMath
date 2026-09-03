#include "NoLib/Complex/ComplexArithmetic.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Scalar/SquareRoot.hpp"

namespace NoLib
{
    ComplexNumber addComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var)
    {
        return makeComplex(left_Complex_Var.real_Real_Var + right_Complex_Var.real_Real_Var, left_Complex_Var.imaginary_Real_Var + right_Complex_Var.imaginary_Real_Var);
    }

    ComplexNumber subtractComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var)
    {
        return makeComplex(left_Complex_Var.real_Real_Var - right_Complex_Var.real_Real_Var, left_Complex_Var.imaginary_Real_Var - right_Complex_Var.imaginary_Real_Var);
    }

    ComplexNumber multiplyComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var)
    {
        return makeComplex(left_Complex_Var.real_Real_Var * right_Complex_Var.real_Real_Var - left_Complex_Var.imaginary_Real_Var * right_Complex_Var.imaginary_Real_Var, left_Complex_Var.real_Real_Var * right_Complex_Var.imaginary_Real_Var + left_Complex_Var.imaginary_Real_Var * right_Complex_Var.real_Real_Var);
    }

    bool divideComplex(const ComplexNumber& numerator_Complex_Var, const ComplexNumber& denominator_Complex_Var, ComplexNumber& result_Complex_Var)
    {
        Real denominator_Real_Var =
            denominator_Complex_Var.real_Real_Var *
                denominator_Complex_Var.real_Real_Var +
            denominator_Complex_Var.imaginary_Real_Var *
                denominator_Complex_Var.imaginary_Real_Var;

        if (denominator_Real_Var == 0.0L)
        {
            return false;
        }

        result_Complex_Var = makeComplex((numerator_Complex_Var.real_Real_Var * denominator_Complex_Var.real_Real_Var + numerator_Complex_Var.imaginary_Real_Var * denominator_Complex_Var.imaginary_Real_Var) / denominator_Real_Var, (numerator_Complex_Var.imaginary_Real_Var * denominator_Complex_Var.real_Real_Var - numerator_Complex_Var.real_Real_Var * denominator_Complex_Var.imaginary_Real_Var) / denominator_Real_Var);
        return true;
    }

    Real complexMagnitude(const ComplexNumber& value_Complex_Var)
    {
        Real realMagnitude_Real_Var =
            absolute(value_Complex_Var.real_Real_Var);
        Real imaginaryMagnitude_Real_Var =
            absolute(value_Complex_Var.imaginary_Real_Var);
        Real scale_Real_Var = maximum(realMagnitude_Real_Var, imaginaryMagnitude_Real_Var);

        if (scale_Real_Var == 0.0L)
        {
            return 0.0L;
        }

        Real realRatio_Real_Var =
            value_Complex_Var.real_Real_Var / scale_Real_Var;
        Real imaginaryRatio_Real_Var =
            value_Complex_Var.imaginary_Real_Var / scale_Real_Var;
        Real root_Real_Var = 0.0L;
        squareRoot(realRatio_Real_Var * realRatio_Real_Var + imaginaryRatio_Real_Var * imaginaryRatio_Real_Var, root_Real_Var);
        return scale_Real_Var * root_Real_Var;
    }
}
