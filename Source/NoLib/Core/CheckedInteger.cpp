#include "NoLib/Core/CheckedInteger.hpp"

namespace NoLib
{
    bool multiplySignedChecked(SLong left_SLong_Var, SLong right_SLong_Var, SLong& result_SLong_Var)
    {
        SLong minimum_SLong_Var = -maximumSigned() - 1;

        if (left_SLong_Var == 0 || right_SLong_Var == 0)
        {
            result_SLong_Var = 0;
            return true;
        }

        if ((left_SLong_Var == -1 && right_SLong_Var == minimum_SLong_Var) || (right_SLong_Var == -1 && left_SLong_Var == minimum_SLong_Var))
        {
            return false;
        }

        if ((left_SLong_Var > 0 && right_SLong_Var > 0 && left_SLong_Var > maximumSigned() / right_SLong_Var) ||
            (left_SLong_Var > 0 && right_SLong_Var < 0 && right_SLong_Var < minimum_SLong_Var / left_SLong_Var) ||
            (left_SLong_Var < 0 && right_SLong_Var > 0 && left_SLong_Var < minimum_SLong_Var / right_SLong_Var) ||
            (left_SLong_Var < 0 && right_SLong_Var < 0 && left_SLong_Var < maximumSigned() / right_SLong_Var))
        {
            return false;
        }

        result_SLong_Var = left_SLong_Var * right_SLong_Var;
        return true;
    }

    bool safeAddUnsigned(ULong left_ULong_Var, ULong right_ULong_Var, ULong& result_ULong_Var)
    {
        if (left_ULong_Var > maximumUnsigned() - right_ULong_Var)
        {
            return false;
        }

        result_ULong_Var = left_ULong_Var + right_ULong_Var;
        return true;
    }

    bool safeMultiplyUnsigned(ULong left_ULong_Var, ULong right_ULong_Var, ULong& result_ULong_Var)
    {
        if (left_ULong_Var != 0 && right_ULong_Var > maximumUnsigned() / left_ULong_Var)
        {
            return false;
        }

        result_ULong_Var = left_ULong_Var * right_ULong_Var;
        return true;
    }
}
