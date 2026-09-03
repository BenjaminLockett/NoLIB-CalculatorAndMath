#include "NoLib/Scalar/Powers.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Core/RealTraits.hpp"
#include "NoLib/Scalar/Logarithm.hpp"
#include "NoLib/Scalar/Exponential.hpp"

namespace NoLib
{
    bool realIntegerPower(Real base_Real_Var, SLong exponent_SLong_Var, Real& result_Real_Var)
    {
        if (base_Real_Var == 0.0L && exponent_SLong_Var <= 0)
        {
            return false;
        }

        bool negativeExponent_Bool_Var = exponent_SLong_Var < 0;
        ULong exponentMagnitude_ULong_Var = negativeExponent_Bool_Var
            ? static_cast<ULong>(-(exponent_SLong_Var + 1)) + 1
            : static_cast<ULong>(exponent_SLong_Var);
        Real factor_Real_Var = base_Real_Var;
        Real power_Real_Var = 1.0L;

        while (exponentMagnitude_ULong_Var > 0)
        {
            if ((exponentMagnitude_ULong_Var & 1ULL) != 0)
            {
                if (!safeMultiplyReal(power_Real_Var, factor_Real_Var, power_Real_Var))
                {
                    return false;
                }
            }

            exponentMagnitude_ULong_Var >>= 1;

            if (exponentMagnitude_ULong_Var > 0)
            {
                if (!safeMultiplyReal(factor_Real_Var, factor_Real_Var, factor_Real_Var))
                {
                    return false;
                }
            }
        }

        result_Real_Var = negativeExponent_Bool_Var
            ? 1.0L / power_Real_Var
            : power_Real_Var;
        return true;
    }

    bool realPower(Real base_Real_Var, Real exponent_Real_Var, Real& result_Real_Var)
    {
        if (base_Real_Var == 0.0L)
        {
            if (exponent_Real_Var <= 0.0L)
            {
                return false;
            }

            result_Real_Var = 0.0L;
            return true;
        }

        if (base_Real_Var < 0.0L)
        {
            Real signedLimit_Real_Var =
                static_cast<Real>(maximumSigned());

            if (exponent_Real_Var > signedLimit_Real_Var || exponent_Real_Var < -signedLimit_Real_Var)
            {
                return false;
            }

            SLong wholeExponent_SLong_Var =
                static_cast<SLong>(exponent_Real_Var);

            if (absolute(exponent_Real_Var - static_cast<Real>(wholeExponent_SLong_Var)) > 1.0e-15L)
            {
                return false;
            }

            return realIntegerPower(base_Real_Var, wholeExponent_SLong_Var, result_Real_Var);
        }

        Real logarithm_Real_Var = 0.0L;
        naturalLog(base_Real_Var, logarithm_Real_Var);
        return exponential(logarithm_Real_Var * exponent_Real_Var, result_Real_Var);
    }

    bool rationalPower(Real base_Real_Var, SLong numerator_SLong_Var, ULong denominator_ULong_Var, Real& result_Real_Var)
    {
        if (denominator_ULong_Var == 0 || (base_Real_Var == 0.0L && numerator_SLong_Var <= 0))
        {
            return false;
        }

        if (base_Real_Var < 0.0L && (denominator_ULong_Var & 1ULL) == 0)
        {
            return false;
        }

        Real magnitude_Real_Var = 0.0L;

        if (!realPower(absolute(base_Real_Var), static_cast<Real>(numerator_SLong_Var) / static_cast<Real>(denominator_ULong_Var), magnitude_Real_Var))
        {
            return false;
        }

        ULong numeratorMagnitude_ULong_Var = numerator_SLong_Var < 0
            ? static_cast<ULong>(-(numerator_SLong_Var + 1)) + 1
            : static_cast<ULong>(numerator_SLong_Var);
        bool negativeResult_Bool_Var =
            base_Real_Var < 0.0L &&
            (numeratorMagnitude_ULong_Var & 1ULL) != 0;

        result_Real_Var = negativeResult_Bool_Var
            ? -magnitude_Real_Var
            : magnitude_Real_Var;
        return true;
    }

    bool scientificRealPower(Real base_Real_Var, Real exponent_Real_Var, ScientificNumber& result_Scientific_Var)
    {
        if (base_Real_Var < 0.0L || (base_Real_Var == 0.0L && exponent_Real_Var <= 0.0L))
        {
            return false;
        }

        if (base_Real_Var == 0.0L)
        {
            result_Scientific_Var.value_Real_Var = 0.0L;
            result_Scientific_Var.power_ULong_Var = 0;
            result_Scientific_Var.negativePower_Bool_Var = false;
            return true;
        }

        const Real lnTen_Real_Var =
            2.302585092994045684017991454684364208L;
        Real logarithm_Real_Var = 0.0L;

        if (!naturalLog(base_Real_Var, logarithm_Real_Var))
        {
            return false;
        }

        Real decimalPower_Real_Var =
            logarithm_Real_Var * exponent_Real_Var / lnTen_Real_Var;
        Real zeroTest_Real_Var = decimalPower_Real_Var * 0.0L;

        if (decimalPower_Real_Var != decimalPower_Real_Var || zeroTest_Real_Var != zeroTest_Real_Var)
        {
            return false;
        }

        bool negativePower_Bool_Var = decimalPower_Real_Var < 0.0L;
        Real magnitude_Real_Var = absolute(decimalPower_Real_Var);

        if (magnitude_Real_Var > static_cast<Real>(maximumUnsigned()))
        {
            return false;
        }

        ULong wholePower_ULong_Var =
            static_cast<ULong>(magnitude_Real_Var);
        Real fraction_Real_Var =
            magnitude_Real_Var -
            static_cast<Real>(wholePower_ULong_Var);
        Real mantissa_Real_Var = 0.0L;
        Real signedFraction_Real_Var = negativePower_Bool_Var
            ? -fraction_Real_Var
            : fraction_Real_Var;

        if (!exponential(signedFraction_Real_Var * lnTen_Real_Var, mantissa_Real_Var))
        {
            return false;
        }

        result_Scientific_Var.value_Real_Var = mantissa_Real_Var;
        result_Scientific_Var.power_ULong_Var = wholePower_ULong_Var;
        result_Scientific_Var.negativePower_Bool_Var =
            negativePower_Bool_Var;
        return normalizeScientific(result_Scientific_Var);
    }

    bool nthRoot(Real value_Real_Var, ULong degree_ULong_Var, Real& result_Real_Var)
    {
        if (degree_ULong_Var == 0 || (value_Real_Var < 0.0L && (degree_ULong_Var & 1ULL) == 0))
        {
            return false;
        }

        if (value_Real_Var == 0.0L)
        {
            result_Real_Var = 0.0L;
            return true;
        }

        Real guess_Real_Var = 0.0L;

        if (!realPower(absolute(value_Real_Var), 1.0L / static_cast<Real>(degree_ULong_Var), guess_Real_Var))
        {
            return false;
        }

        if (value_Real_Var < 0.0L)
        {
            guess_Real_Var = -guess_Real_Var;
        }

        if (degree_ULong_Var <= static_cast<ULong>(maximumSigned()))
        {
            for (UInt index_UInt_Var = 0; index_UInt_Var < 8; ++index_UInt_Var)
            {
                Real denominator_Real_Var = 0.0L;

                if (!realIntegerPower(guess_Real_Var, static_cast<SLong>(degree_ULong_Var - 1), denominator_Real_Var) || denominator_Real_Var == 0.0L)
                {
                    break;
                }

                guess_Real_Var =
                    ((static_cast<Real>(degree_ULong_Var) - 1.0L) * guess_Real_Var + value_Real_Var / denominator_Real_Var) /
                    static_cast<Real>(degree_ULong_Var);
            }
        }

        result_Real_Var = guess_Real_Var;
        return true;
    }
}
