#pragma once

// Range and safety facts about the Real type.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    Real maximumReal();

    bool safeMultiplyReal(Real left_Real_Var, Real right_Real_Var, Real& result_Real_Var);

    UInt maximumBinaryScale();
}
