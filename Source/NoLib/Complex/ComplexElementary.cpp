#include "NoLib/Complex/ComplexElementary.hpp"
#include "NoLib/Complex/ComplexArithmetic.hpp"
#include "NoLib/Scalar/Exponential.hpp"
#include "NoLib/Scalar/Logarithm.hpp"
#include "NoLib/Scalar/Trigonometry.hpp"

namespace NoLib
{
    bool complexExponential(const ComplexNumber& value_Complex_Var, ComplexNumber& result_Complex_Var)
    {
        Real magnitude_Real_Var = 0.0L;

        if (!exponential(value_Complex_Var.real_Real_Var, magnitude_Real_Var))
        {
            return false;
        }

        result_Complex_Var = makeComplex(magnitude_Real_Var * cosine(value_Complex_Var.imaginary_Real_Var), magnitude_Real_Var * sine(value_Complex_Var.imaginary_Real_Var));
        return true;
    }

    bool complexLogarithm(const ComplexNumber& value_Complex_Var, ComplexNumber& result_Complex_Var)
    {
        Real magnitude_Real_Var = complexMagnitude(value_Complex_Var);

        if (magnitude_Real_Var == 0.0L)
        {
            return false;
        }

        Real logarithm_Real_Var = 0.0L;
        naturalLog(magnitude_Real_Var, logarithm_Real_Var);
        result_Complex_Var = makeComplex(logarithm_Real_Var, arcTangent2(value_Complex_Var.imaginary_Real_Var, value_Complex_Var.real_Real_Var));
        return true;
    }

    bool complexPower(const ComplexNumber& base_Complex_Var, const ComplexNumber& exponent_Complex_Var, ComplexNumber& result_Complex_Var)
    {
        ComplexNumber logarithm_Complex_Var;

        if (!complexLogarithm(base_Complex_Var, logarithm_Complex_Var))
        {
            if (exponent_Complex_Var.imaginary_Real_Var == 0.0L && exponent_Complex_Var.real_Real_Var > 0.0L)
            {
                result_Complex_Var = makeComplex(0.0L, 0.0L);
                return true;
            }

            return false;
        }

        ComplexNumber product_Complex_Var =
            multiplyComplex(exponent_Complex_Var, logarithm_Complex_Var);
        return complexExponential(product_Complex_Var, result_Complex_Var);
    }
}
