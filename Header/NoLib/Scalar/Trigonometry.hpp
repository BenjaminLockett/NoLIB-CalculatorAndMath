#pragma once

// Range reduction, sine, cosine and arc tangent.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    Real reduceAngle(Real angle_Real_Var);

    Real sine(Real angle_Real_Var);

    Real cosine(Real angle_Real_Var);

    Real arcTangent(Real value_Real_Var);

    Real arcTangent2(Real y_Real_Var, Real x_Real_Var);
}
