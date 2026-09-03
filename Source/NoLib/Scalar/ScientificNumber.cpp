#include "NoLib/Scalar/ScientificNumber.hpp"
#include "NoLib/Core/CheckedInteger.hpp"
#include "NoLib/Text/RealFormatting.hpp"
#include "NoLib/Text/IntegerFormatting.hpp"
#include "NoLib/Core/Utility.hpp"

namespace NoLib
{
    bool addScientificPowers(bool leftNegative_Bool_Var, ULong leftPower_ULong_Var, bool rightNegative_Bool_Var, ULong rightPower_ULong_Var, bool& resultNegative_Bool_Var, ULong& resultPower_ULong_Var)
    {
        if (leftNegative_Bool_Var == rightNegative_Bool_Var)
        {
            resultNegative_Bool_Var = leftNegative_Bool_Var;
            return safeAddUnsigned(leftPower_ULong_Var, rightPower_ULong_Var, resultPower_ULong_Var);
        }

        if (leftPower_ULong_Var >= rightPower_ULong_Var)
        {
            resultPower_ULong_Var =
                leftPower_ULong_Var - rightPower_ULong_Var;
            resultNegative_Bool_Var = leftNegative_Bool_Var;
        }
        else
        {
            resultPower_ULong_Var =
                rightPower_ULong_Var - leftPower_ULong_Var;
            resultNegative_Bool_Var = rightNegative_Bool_Var;
        }

        if (resultPower_ULong_Var == 0)
        {
            resultNegative_Bool_Var = false;
        }

        return true;
    }

    bool changeScientificPower(ScientificNumber& number_Scientific_Var, bool negative_Bool_Var, ULong amount_ULong_Var)
    {
        bool nextNegative_Bool_Var = false;
        ULong nextPower_ULong_Var = 0;

        if (!addScientificPowers(number_Scientific_Var.negativePower_Bool_Var, number_Scientific_Var.power_ULong_Var, negative_Bool_Var, amount_ULong_Var, nextNegative_Bool_Var, nextPower_ULong_Var))
        {
            return false;
        }

        number_Scientific_Var.negativePower_Bool_Var =
            nextNegative_Bool_Var;
        number_Scientific_Var.power_ULong_Var = nextPower_ULong_Var;
        return true;
    }

    bool normalizeScientific(ScientificNumber& number_Scientific_Var)
    {
        if (number_Scientific_Var.value_Real_Var == 0.0L)
        {
            number_Scientific_Var.power_ULong_Var = 0;
            number_Scientific_Var.negativePower_Bool_Var = false;
            return true;
        }

        Real magnitude_Real_Var =
            absolute(number_Scientific_Var.value_Real_Var);

        while (magnitude_Real_Var >= 10.0L)
        {
            number_Scientific_Var.value_Real_Var /= 10.0L;
            magnitude_Real_Var /= 10.0L;

            if (!changeScientificPower(number_Scientific_Var, false, 1))
            {
                return false;
            }
        }

        while (magnitude_Real_Var < 1.0L)
        {
            number_Scientific_Var.value_Real_Var *= 10.0L;
            magnitude_Real_Var *= 10.0L;

            if (!changeScientificPower(number_Scientific_Var, true, 1))
            {
                return false;
            }
        }

        return true;
    }

    ScientificNumber makeScientific(ULong value_ULong_Var)
    {
        ScientificNumber number_Scientific_Var;
        number_Scientific_Var.value_Real_Var =
            static_cast<Real>(value_ULong_Var);
        number_Scientific_Var.power_ULong_Var = 0;
        number_Scientific_Var.negativePower_Bool_Var = false;
        normalizeScientific(number_Scientific_Var);
        return number_Scientific_Var;
    }

    bool multiplyScientific(const ScientificNumber& left_Scientific_Var, const ScientificNumber& right_Scientific_Var, ScientificNumber& result_Scientific_Var)
    {
        result_Scientific_Var.value_Real_Var =
            left_Scientific_Var.value_Real_Var *
            right_Scientific_Var.value_Real_Var;

        if (!addScientificPowers(left_Scientific_Var.negativePower_Bool_Var, left_Scientific_Var.power_ULong_Var, right_Scientific_Var.negativePower_Bool_Var, right_Scientific_Var.power_ULong_Var, result_Scientific_Var.negativePower_Bool_Var, result_Scientific_Var.power_ULong_Var))
        {
            return false;
        }

        return normalizeScientific(result_Scientific_Var);
    }

    bool scientificPower(ULong base_ULong_Var, ULong exponent_ULong_Var, ScientificNumber& result_Scientific_Var)
    {
        if (base_ULong_Var == 0 && exponent_ULong_Var == 0)
        {
            return false;
        }

        result_Scientific_Var.value_Real_Var =
            exponent_ULong_Var == 0 ? 1.0L : 0.0L;
        result_Scientific_Var.power_ULong_Var = 0;
        result_Scientific_Var.negativePower_Bool_Var = false;

        if (base_ULong_Var == 0 || exponent_ULong_Var == 0)
        {
            return true;
        }

        result_Scientific_Var.value_Real_Var = 1.0L;
        ScientificNumber factor_Scientific_Var =
            makeScientific(base_ULong_Var);

        while (exponent_ULong_Var > 0)
        {
            if ((exponent_ULong_Var & 1ULL) != 0)
            {
                ScientificNumber next_Scientific_Var;

                if (!multiplyScientific(result_Scientific_Var, factor_Scientific_Var, next_Scientific_Var))
                {
                    return false;
                }

                result_Scientific_Var = next_Scientific_Var;
            }

            exponent_ULong_Var >>= 1;

            if (exponent_ULong_Var > 0)
            {
                ScientificNumber next_Scientific_Var;

                if (!multiplyScientific(factor_Scientific_Var, factor_Scientific_Var, next_Scientific_Var))
                {
                    return false;
                }

                factor_Scientific_Var = next_Scientific_Var;
            }
        }

        return true;
    }

    void largestExactPower(ULong base_ULong_Var, ULong exponent_ULong_Var, ULong& result_ULong_Var, ULong& exactExponent_ULong_Var)
    {
        if (exponent_ULong_Var == 0)
        {
            result_ULong_Var = 1;
            exactExponent_ULong_Var = 0;
            return;
        }

        if (base_ULong_Var == 0)
        {
            result_ULong_Var = 0;
            exactExponent_ULong_Var = exponent_ULong_Var;
            return;
        }

        if (base_ULong_Var == 1)
        {
            result_ULong_Var = 1;
            exactExponent_ULong_Var = exponent_ULong_Var;
            return;
        }

        result_ULong_Var = 1;
        exactExponent_ULong_Var = 0;

        while (exactExponent_ULong_Var < exponent_ULong_Var)
        {
            ULong next_ULong_Var = 0;

            if (!safeMultiplyUnsigned(result_ULong_Var, base_ULong_Var, next_ULong_Var))
            {
                break;
            }

            result_ULong_Var = next_ULong_Var;
            ++exactExponent_ULong_Var;
        }
    }

    String scientificToString(const ScientificNumber& number_Scientific_Var, UInt decimals_UInt_Var)
    {
        String result_String_Var =
            realToString(number_Scientific_Var.value_Real_Var, decimals_UInt_Var);
        result_String_Var += " x 10^";

        if (number_Scientific_Var.negativePower_Bool_Var && number_Scientific_Var.power_ULong_Var != 0)
        {
            result_String_Var += '-';
        }

        result_String_Var +=
            unsignedToString(number_Scientific_Var.power_ULong_Var);
        return result_String_Var;
    }
}
