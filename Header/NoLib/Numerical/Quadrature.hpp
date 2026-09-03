#pragma once

// Composite Simpson integration.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Core/RealTraits.hpp"

namespace NoLib
{
    template <typename Function_T>
    Real integrateSimpson(Function_T function_Function_Var, Real start_Real_Var, Real end_Real_Var, UInt intervals_UInt_Var = 1000)
    {
        if (intervals_UInt_Var < 2)
        {
            intervals_UInt_Var = 2;
        }

        if ((intervals_UInt_Var & 1U) != 0)
        {
            ++intervals_UInt_Var;
        }

        Real step_Real_Var =
            (end_Real_Var - start_Real_Var) /
            static_cast<Real>(intervals_UInt_Var);
        Real sum_Real_Var =
            function_Function_Var(start_Real_Var) +
            function_Function_Var(end_Real_Var);

        for (UInt index_UInt_Var = 1; index_UInt_Var < intervals_UInt_Var; ++index_UInt_Var)
        {
            Real x_Real_Var =
                start_Real_Var + index_UInt_Var * step_Real_Var;
            sum_Real_Var +=
                ((index_UInt_Var & 1U) != 0 ? 4.0L : 2.0L) *
                function_Function_Var(x_Real_Var);
        }

        return sum_Real_Var * step_Real_Var / 3.0L;
    }
}
