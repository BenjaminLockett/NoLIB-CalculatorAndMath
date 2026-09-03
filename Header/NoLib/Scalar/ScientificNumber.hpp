#pragma once

// Mantissa and signed-magnitude decimal exponent arithmetic.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Text/String.hpp"

namespace NoLib
{
    struct ScientificNumber
    {
        Real value_Real_Var;
        ULong power_ULong_Var;
        bool negativePower_Bool_Var;
    };

    bool addScientificPowers(bool leftNegative_Bool_Var, ULong leftPower_ULong_Var, bool rightNegative_Bool_Var, ULong rightPower_ULong_Var, bool& resultNegative_Bool_Var, ULong& resultPower_ULong_Var);

    bool changeScientificPower(ScientificNumber& number_Scientific_Var, bool negative_Bool_Var, ULong amount_ULong_Var);

    bool normalizeScientific(ScientificNumber& number_Scientific_Var);

    ScientificNumber makeScientific(ULong value_ULong_Var);

    bool multiplyScientific(const ScientificNumber& left_Scientific_Var, const ScientificNumber& right_Scientific_Var, ScientificNumber& result_Scientific_Var);

    bool scientificPower(ULong base_ULong_Var, ULong exponent_ULong_Var, ScientificNumber& result_Scientific_Var);

    void largestExactPower(ULong base_ULong_Var, ULong exponent_ULong_Var, ULong& result_ULong_Var, ULong& exactExponent_ULong_Var);

    String scientificToString(const ScientificNumber& number_Scientific_Var, UInt decimals_UInt_Var = 18);
}
