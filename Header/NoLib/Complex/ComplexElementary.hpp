#pragma once

// Complex exponential, logarithm and power.

#include "NoLib/Complex/ComplexNumber.hpp"

namespace NoLib
{
    bool complexExponential(const ComplexNumber& value_Complex_Var, ComplexNumber& result_Complex_Var);

    bool complexLogarithm(const ComplexNumber& value_Complex_Var, ComplexNumber& result_Complex_Var);

    bool complexPower(const ComplexNumber& base_Complex_Var, const ComplexNumber& exponent_Complex_Var, ComplexNumber& result_Complex_Var);
}
