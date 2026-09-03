#pragma once

// Explicit Euler stepper for first-order equations.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    template <typename Derivative_T>
    Real solveEuler(Derivative_T derivative_Derivative_Var, Real startX_Real_Var, Real startY_Real_Var, Real targetX_Real_Var, UInt steps_UInt_Var)
    {
        if (steps_UInt_Var == 0)
        {
            return startY_Real_Var;
        }

        Real step_Real_Var =
            (targetX_Real_Var - startX_Real_Var) /
            static_cast<Real>(steps_UInt_Var);
        Real x_Real_Var = startX_Real_Var;
        Real y_Real_Var = startY_Real_Var;

        for (UInt index_UInt_Var = 0; index_UInt_Var < steps_UInt_Var; ++index_UInt_Var)
        {
            y_Real_Var +=
                step_Real_Var *
                derivative_Derivative_Var(x_Real_Var, y_Real_Var);
            x_Real_Var += step_Real_Var;
        }

        return y_Real_Var;
    }
}
