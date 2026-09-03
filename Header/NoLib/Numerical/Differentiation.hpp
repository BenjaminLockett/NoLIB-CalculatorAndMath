#pragma once

// Centred-difference derivative approximation.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    template <typename Function_T>
    Real derivativeAt(Function_T function_Function_Var, Real x_Real_Var, Real step_Real_Var = 1.0e-6L)
    {
        return (function_Function_Var(x_Real_Var + step_Real_Var) - function_Function_Var(x_Real_Var - step_Real_Var)) /
               (2.0L * step_Real_Var);
    }
}
