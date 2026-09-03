#include "NoLib/Complex/ComplexFormatting.hpp"
#include "NoLib/Text/RealFormatting.hpp"

namespace NoLib
{
    String complexToString(const ComplexNumber& value_Complex_Var, UInt decimals_UInt_Var)
    {
        String result_String_Var =
            realToString(value_Complex_Var.real_Real_Var, decimals_UInt_Var);

        if (value_Complex_Var.imaginary_Real_Var >= 0.0L)
        {
            result_String_Var += " + ";
            result_String_Var +=
                realToString(value_Complex_Var.imaginary_Real_Var, decimals_UInt_Var);
        }
        else
        {
            result_String_Var += " - ";
            result_String_Var +=
                realToString(-value_Complex_Var.imaginary_Real_Var, decimals_UInt_Var);
        }

        result_String_Var += 'i';
        return result_String_Var;
    }
}
