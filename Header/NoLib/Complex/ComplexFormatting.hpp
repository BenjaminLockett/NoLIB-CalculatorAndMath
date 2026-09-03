#pragma once

// Human readable complex output.

#include "NoLib/Complex/ComplexNumber.hpp"
#include "NoLib/Text/String.hpp"

namespace NoLib
{
    String complexToString(const ComplexNumber& value_Complex_Var, UInt decimals_UInt_Var = 12);
}
