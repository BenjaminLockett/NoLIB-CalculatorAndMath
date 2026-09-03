#pragma once

// Add, subtract, multiply, divide and magnitude.

#include "NoLib/Complex/ComplexNumber.hpp"

namespace NoLib
{
    ComplexNumber addComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var);

    ComplexNumber subtractComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var);

    ComplexNumber multiplyComplex(const ComplexNumber& left_Complex_Var, const ComplexNumber& right_Complex_Var);

    bool divideComplex(const ComplexNumber& numerator_Complex_Var, const ComplexNumber& denominator_Complex_Var, ComplexNumber& result_Complex_Var);

    Real complexMagnitude(const ComplexNumber& value_Complex_Var);
}
