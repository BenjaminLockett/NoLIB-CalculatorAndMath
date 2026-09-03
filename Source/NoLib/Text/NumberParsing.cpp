#include "NoLib/Text/NumberParsing.hpp"

namespace NoLib
{
    bool readUnsigned(const String& text_String_Var, ULong& result_ULong_Var)
    {
        if (text_String_Var.empty())
        {
            return false;
        }

        result_ULong_Var = 0;

        for (ULong index_ULong_Var = 0; index_ULong_Var < text_String_Var.length(); ++index_ULong_Var)
        {
            char character_Char_Var = text_String_Var[index_ULong_Var];

            if (character_Char_Var < '0' || character_Char_Var > '9')
            {
                return false;
            }

            UInt digit_UInt_Var =
                static_cast<UInt>(character_Char_Var - '0');

            if (result_ULong_Var > (maximumUnsigned() - digit_UInt_Var) / 10)
            {
                return false;
            }

            result_ULong_Var = result_ULong_Var * 10 + digit_UInt_Var;
        }

        return true;
    }

    bool readSigned(const String& text_String_Var, SLong& result_SLong_Var)
    {
        if (text_String_Var.empty())
        {
            return false;
        }

        bool negative_Bool_Var = text_String_Var[0] == '-';
        ULong start_ULong_Var = negative_Bool_Var || text_String_Var[0] == '+'
            ? 1
            : 0;

        if (start_ULong_Var == text_String_Var.length())
        {
            return false;
        }

        ULong magnitude_ULong_Var = 0;
        ULong maximumMagnitude_ULong_Var = negative_Bool_Var
            ? static_cast<ULong>(maximumSigned()) + 1
            : static_cast<ULong>(maximumSigned());

        for (ULong index_ULong_Var = start_ULong_Var; index_ULong_Var < text_String_Var.length(); ++index_ULong_Var)
        {
            char character_Char_Var = text_String_Var[index_ULong_Var];

            if (character_Char_Var < '0' || character_Char_Var > '9')
            {
                return false;
            }

            UInt digit_UInt_Var =
                static_cast<UInt>(character_Char_Var - '0');

            if (magnitude_ULong_Var > (maximumMagnitude_ULong_Var - digit_UInt_Var) / 10)
            {
                return false;
            }

            magnitude_ULong_Var =
                magnitude_ULong_Var * 10 + digit_UInt_Var;
        }

        if (!negative_Bool_Var)
        {
            result_SLong_Var = static_cast<SLong>(magnitude_ULong_Var);
        }
        else if (magnitude_ULong_Var == static_cast<ULong>(maximumSigned()) + 1)
        {
            result_SLong_Var = -maximumSigned() - 1;
        }
        else
        {
            result_SLong_Var = -static_cast<SLong>(magnitude_ULong_Var);
        }

        return true;
    }

    bool readReal(const String& text_String_Var, Real& result_Real_Var)
    {
        if (text_String_Var.empty())
        {
            return false;
        }

        ULong index_ULong_Var = 0;
        bool negative_Bool_Var = false;

        if (text_String_Var[index_ULong_Var] == '-' || text_String_Var[index_ULong_Var] == '+')
        {
            negative_Bool_Var = text_String_Var[index_ULong_Var] == '-';
            ++index_ULong_Var;
        }

        bool foundDigit_Bool_Var = false;
        bool foundPoint_Bool_Var = false;
        Real value_Real_Var = 0.0L;
        Real decimalPlace_Real_Var = 0.1L;

        while (index_ULong_Var < text_String_Var.length())
        {
            char character_Char_Var = text_String_Var[index_ULong_Var];

            if (character_Char_Var == 'e' || character_Char_Var == 'E')
            {
                break;
            }

            if (character_Char_Var == '.')
            {
                if (foundPoint_Bool_Var)
                {
                    return false;
                }

                foundPoint_Bool_Var = true;
                ++index_ULong_Var;
                continue;
            }

            if (character_Char_Var < '0' || character_Char_Var > '9')
            {
                return false;
            }

            foundDigit_Bool_Var = true;
            UInt digit_UInt_Var =
                static_cast<UInt>(character_Char_Var - '0');

            if (!foundPoint_Bool_Var)
            {
                Real next_Real_Var =
                    value_Real_Var * 10.0L + digit_UInt_Var;
                Real zeroTest_Real_Var = next_Real_Var * 0.0L;

                if (!(next_Real_Var >= value_Real_Var) || zeroTest_Real_Var != zeroTest_Real_Var)
                {
                    return false;
                }

                value_Real_Var = next_Real_Var;
            }
            else
            {
                value_Real_Var += digit_UInt_Var * decimalPlace_Real_Var;
                decimalPlace_Real_Var *= 0.1L;
            }

            ++index_ULong_Var;
        }

        if (!foundDigit_Bool_Var)
        {
            return false;
        }

        SLong exponent_SLong_Var = 0;

        if (index_ULong_Var < text_String_Var.length())
        {
            ++index_ULong_Var;
            String exponent_String_Var;

            while (index_ULong_Var < text_String_Var.length())
            {
                exponent_String_Var += text_String_Var[index_ULong_Var];
                ++index_ULong_Var;
            }

            if (!readSigned(exponent_String_Var, exponent_SLong_Var))
            {
                return false;
            }
        }

        if (exponent_SLong_Var > 100000 || exponent_SLong_Var < -100000)
        {
            return false;
        }

        while (exponent_SLong_Var > 0)
        {
            Real next_Real_Var = value_Real_Var * 10.0L;
            Real zeroTest_Real_Var = next_Real_Var * 0.0L;

            if (!(next_Real_Var >= value_Real_Var) || zeroTest_Real_Var != zeroTest_Real_Var)
            {
                return false;
            }

            value_Real_Var = next_Real_Var;
            --exponent_SLong_Var;
        }

        while (exponent_SLong_Var < 0)
        {
            value_Real_Var /= 10.0L;
            ++exponent_SLong_Var;
        }

        result_Real_Var = negative_Bool_Var
            ? -value_Real_Var
            : value_Real_Var;
        return true;
    }
}
