#include "NoLib/Text/RealFormatting.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Text/IntegerFormatting.hpp"

namespace NoLib
{
    String realToString(Real value_Real_Var, UInt decimals_UInt_Var)
    {
        if (value_Real_Var != value_Real_Var)
        {
            return String("nan");
        }

        Real zeroTest_Real_Var = value_Real_Var * 0.0L;

        if (zeroTest_Real_Var != zeroTest_Real_Var)
        {
            return value_Real_Var < 0.0L
                ? String("-inf")
                : String("inf");
        }

        if (value_Real_Var == 0.0L)
        {
            return String("0");
        }

        bool negative_Bool_Var = value_Real_Var < 0.0L;
        Real working_Real_Var = absolute(value_Real_Var);
        SLong exponent_SLong_Var = 0;

        while (working_Real_Var >= 10.0L)
        {
            working_Real_Var /= 10.0L;
            ++exponent_SLong_Var;
        }

        while (working_Real_Var < 1.0L)
        {
            working_Real_Var *= 10.0L;
            --exponent_SLong_Var;
        }

        String result_String_Var;

        if (negative_Bool_Var)
        {
            result_String_Var += '-';
        }

        UInt firstDigit_UInt_Var = static_cast<UInt>(working_Real_Var);
        result_String_Var += static_cast<char>('0' + firstDigit_UInt_Var);
        working_Real_Var -= firstDigit_UInt_Var;

        if (decimals_UInt_Var > 0)
        {
            result_String_Var += '.';

            for (UInt index_UInt_Var = 0; index_UInt_Var < decimals_UInt_Var; ++index_UInt_Var)
            {
                working_Real_Var *= 10.0L;
                UInt digit_UInt_Var = static_cast<UInt>(working_Real_Var);
                result_String_Var += static_cast<char>('0' + digit_UInt_Var);
                working_Real_Var -= digit_UInt_Var;
            }
        }

        result_String_Var += 'e';
        result_String_Var += exponent_SLong_Var < 0 ? '-' : '+';

        ULong exponentMagnitude_ULong_Var = exponent_SLong_Var < 0
            ? static_cast<ULong>(-(exponent_SLong_Var + 1)) + 1
            : static_cast<ULong>(exponent_SLong_Var);

        result_String_Var += unsignedToString(exponentMagnitude_ULong_Var);
        return result_String_Var;
    }

    String fixedRealToString(Real value_Real_Var, UInt decimals_UInt_Var)
    {
        if (value_Real_Var != value_Real_Var)
        {
            return String("nan");
        }

        Real zeroTest_Real_Var = value_Real_Var * 0.0L;

        if (zeroTest_Real_Var != zeroTest_Real_Var)
        {
            return value_Real_Var < 0.0L ? String("-inf") : String("inf");
        }

        if (value_Real_Var == 0.0L)
        {
            return String("0");
        }

        Real magnitude_Real_Var = absolute(value_Real_Var);

        if (magnitude_Real_Var > static_cast<Real>(maximumUnsigned()) || magnitude_Real_Var < 1.0e-9L)
        {
            return realToString(value_Real_Var, decimals_UInt_Var);
        }

        decimals_UInt_Var = minimum(decimals_UInt_Var, 18U);
        Real rounding_Real_Var = 0.5L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < decimals_UInt_Var; ++index_UInt_Var)
        {
            rounding_Real_Var /= 10.0L;
        }

        magnitude_Real_Var += rounding_Real_Var;
        ULong whole_ULong_Var = static_cast<ULong>(magnitude_Real_Var);
        Real fraction_Real_Var = magnitude_Real_Var - static_cast<Real>(whole_ULong_Var);
        String result_String_Var;

        if (value_Real_Var < 0.0L)
        {
            result_String_Var += '-';
        }

        result_String_Var += unsignedToString(whole_ULong_Var);

        if (decimals_UInt_Var == 0)
        {
            return result_String_Var;
        }

        result_String_Var += '.';

        for (UInt index_UInt_Var = 0; index_UInt_Var < decimals_UInt_Var; ++index_UInt_Var)
        {
            fraction_Real_Var *= 10.0L;
            UInt digit_UInt_Var = static_cast<UInt>(fraction_Real_Var);
            result_String_Var += static_cast<char>('0' + minimum(digit_UInt_Var, 9U));
            fraction_Real_Var -= static_cast<Real>(digit_UInt_Var);
        }

        while (result_String_Var.back() == '0')
        {
            result_String_Var.popBack();
        }

        if (result_String_Var.back() == '.')
        {
            result_String_Var.popBack();
        }

        return result_String_Var;
    }

    String niceRealToString(Real value_Real_Var, Real tolerance_Real_Var, UInt maximumDenominator_UInt_Var)
    {
        if (absolute(value_Real_Var) <= tolerance_Real_Var)
        {
            return String("0");
        }

        if (value_Real_Var <= static_cast<Real>(maximumSigned()) && value_Real_Var >= static_cast<Real>(-maximumSigned()) - 1.0L)
        {
            SLong truncated_SLong_Var = static_cast<SLong>(value_Real_Var);
            SLong nearest_SLong_Var = truncated_SLong_Var;
            Real difference_Real_Var = value_Real_Var - static_cast<Real>(truncated_SLong_Var);

            if (difference_Real_Var >= 0.5L && truncated_SLong_Var < maximumSigned())
            {
                nearest_SLong_Var = truncated_SLong_Var + 1;
            }
            else if (difference_Real_Var <= -0.5L && truncated_SLong_Var > -maximumSigned() - 1)
            {
                nearest_SLong_Var = truncated_SLong_Var - 1;
            }

            if (absolute(value_Real_Var - static_cast<Real>(nearest_SLong_Var)) <= tolerance_Real_Var)
            {
                return signedToString(nearest_SLong_Var);
            }
        }

        for (UInt denominator_UInt_Var = 2; denominator_UInt_Var <= maximumDenominator_UInt_Var; ++denominator_UInt_Var)
        {
            Real scaled_Real_Var = value_Real_Var * static_cast<Real>(denominator_UInt_Var);

            if (scaled_Real_Var > static_cast<Real>(maximumSigned()) || scaled_Real_Var < static_cast<Real>(-maximumSigned()) - 1.0L)
            {
                continue;
            }

            SLong numerator_SLong_Var = static_cast<SLong>(scaled_Real_Var);
            Real difference_Real_Var = scaled_Real_Var - static_cast<Real>(numerator_SLong_Var);

            if (difference_Real_Var >= 0.5L && numerator_SLong_Var < maximumSigned())
            {
                ++numerator_SLong_Var;
            }
            else if (difference_Real_Var <= -0.5L && numerator_SLong_Var > -maximumSigned() - 1)
            {
                --numerator_SLong_Var;
            }

            if (absolute(value_Real_Var - static_cast<Real>(numerator_SLong_Var) / static_cast<Real>(denominator_UInt_Var)) <= tolerance_Real_Var)
            {
                ULong numeratorMagnitude_ULong_Var = numerator_SLong_Var < 0 ? static_cast<ULong>(-(numerator_SLong_Var + 1)) + 1 : static_cast<ULong>(numerator_SLong_Var);
                ULong divisor_ULong_Var = greatestCommonDivisor(numeratorMagnitude_ULong_Var, denominator_UInt_Var);
                numeratorMagnitude_ULong_Var /= divisor_ULong_Var;
                ULong reducedDenominator_ULong_Var = denominator_UInt_Var / divisor_ULong_Var;
                String result_String_Var;

                if (numerator_SLong_Var < 0)
                {
                    result_String_Var += '-';
                }

                result_String_Var += unsignedToString(numeratorMagnitude_ULong_Var);
                result_String_Var += '/';
                result_String_Var += unsignedToString(reducedDenominator_ULong_Var);
                return result_String_Var;
            }
        }

        return fixedRealToString(value_Real_Var, 10);
    }
}
