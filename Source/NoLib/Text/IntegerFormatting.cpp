#include "NoLib/Text/IntegerFormatting.hpp"

namespace NoLib
{
    String unsignedToString(ULong value_ULong_Var)
    {
        char digits_CharArray_Var[32];
        UInt length_UInt_Var = 0;

        do
        {
            digits_CharArray_Var[length_UInt_Var] =
                static_cast<char>('0' + value_ULong_Var % 10);
            value_ULong_Var /= 10;
            ++length_UInt_Var;
        }
        while (value_ULong_Var != 0);

        String result_String_Var;

        while (length_UInt_Var > 0)
        {
            --length_UInt_Var;
            result_String_Var += digits_CharArray_Var[length_UInt_Var];
        }

        return result_String_Var;
    }

    String signedToString(SLong value_SLong_Var)
    {
        bool negative_Bool_Var = value_SLong_Var < 0;
        ULong magnitude_ULong_Var = negative_Bool_Var
            ? static_cast<ULong>(-(value_SLong_Var + 1)) + 1
            : static_cast<ULong>(value_SLong_Var);

        String result_String_Var;

        if (negative_Bool_Var)
        {
            result_String_Var += '-';
        }

        result_String_Var += unsignedToString(magnitude_ULong_Var);
        return result_String_Var;
    }

    ULong greatestCommonDivisor(ULong left_ULong_Var, ULong right_ULong_Var)
    {
        while (right_ULong_Var != 0)
        {
            ULong remainder_ULong_Var = left_ULong_Var % right_ULong_Var;
            left_ULong_Var = right_ULong_Var;
            right_ULong_Var = remainder_ULong_Var;
        }

        return left_ULong_Var == 0 ? 1 : left_ULong_Var;
    }
}
