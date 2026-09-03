#pragma once

// Integer, real, rational, scientific powers and nth roots.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Scalar/ScientificNumber.hpp"

namespace NoLib
{
    bool realIntegerPower(Real base_Real_Var, SLong exponent_SLong_Var, Real& result_Real_Var);

    bool realPower(Real base_Real_Var, Real exponent_Real_Var, Real& result_Real_Var);

    bool rationalPower(Real base_Real_Var, SLong numerator_SLong_Var, ULong denominator_ULong_Var, Real& result_Real_Var);

    bool scientificRealPower(Real base_Real_Var, Real exponent_Real_Var, ScientificNumber& result_Scientific_Var);

    bool nthRoot(Real value_Real_Var, ULong degree_ULong_Var, Real& result_Real_Var);
}
