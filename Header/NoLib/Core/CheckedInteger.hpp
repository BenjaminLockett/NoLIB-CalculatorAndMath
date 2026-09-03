#pragma once

// Overflow-checked integer arithmetic.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    bool multiplySignedChecked(SLong left_SLong_Var, SLong right_SLong_Var, SLong& result_SLong_Var);

    bool safeAddUnsigned(ULong left_ULong_Var, ULong right_ULong_Var, ULong& result_ULong_Var);

    bool safeMultiplyUnsigned(ULong left_ULong_Var, ULong right_ULong_Var, ULong& result_ULong_Var);
}
