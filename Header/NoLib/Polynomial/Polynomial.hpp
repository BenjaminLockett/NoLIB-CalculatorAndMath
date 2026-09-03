#pragma once

// Fixed-capacity dense polynomial representation.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    // Coefficients are stored from x^0 upward in a fixed raw array.
    template <UInt MaximumDegree_UInt>
    struct Polynomial
    {
        UInt degree_UInt_Var;
        Real coefficients_RealArray_Var[MaximumDegree_UInt + 1];

        Polynomial()
            : degree_UInt_Var(0)
        {
            for (UInt index_UInt_Var = 0; index_UInt_Var <= MaximumDegree_UInt; ++index_UInt_Var)
            {
                coefficients_RealArray_Var[index_UInt_Var] = 0.0L;
            }
        }
    };
}
