#pragma once

// Real to text conversion, including fraction-aware output.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Text/String.hpp"

namespace NoLib
{
    String realToString(Real value_Real_Var, UInt decimals_UInt_Var = 12);

    String fixedRealToString(Real value_Real_Var, UInt decimals_UInt_Var = 10);

    String niceRealToString(Real value_Real_Var, Real tolerance_Real_Var = 1.0e-10L, UInt maximumDenominator_UInt_Var = 64);
}
