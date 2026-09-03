#include "NoLib/Complex/ComplexNumber.hpp"

namespace NoLib
{
    ComplexNumber makeComplex(Real real_Real_Var, Real imaginary_Real_Var)
    {
        ComplexNumber result_Complex_Var;
        result_Complex_Var.real_Real_Var = real_Real_Var;
        result_Complex_Var.imaginary_Real_Var = imaginary_Real_Var;
        return result_Complex_Var;
    }
}
