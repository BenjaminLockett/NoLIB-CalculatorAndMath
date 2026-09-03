#pragma once

// Integer to text conversion and the greatest common divisor.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Text/String.hpp"

namespace NoLib
{
    String unsignedToString(ULong value_ULong_Var);

    String signedToString(SLong value_SLong_Var);

    ULong greatestCommonDivisor(ULong left_ULong_Var, ULong right_ULong_Var);
}
