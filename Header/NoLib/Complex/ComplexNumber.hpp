#pragma once

// Complex representation and construction.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    // Real and imaginary components.
    struct ComplexNumber
    {
        Real real_Real_Var;
        Real imaginary_Real_Var;
    };

    ComplexNumber makeComplex(Real real_Real_Var, Real imaginary_Real_Var = 0.0L);
}
