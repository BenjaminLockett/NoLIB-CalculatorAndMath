#pragma once

// Newton square root with a scaled starting estimate.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    bool squareRoot(Real value_Real_Var, Real& result_Real_Var, UInt iterations_UInt_Var = 80);
}
