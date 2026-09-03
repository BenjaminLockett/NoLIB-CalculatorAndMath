#include "NoLib/Core/RealTraits.hpp"
#include "NoLib/Core/Utility.hpp"

namespace NoLib
{
    Real maximumReal()
    {
        Real current_Real_Var = 1.0L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < 100000; ++index_UInt_Var)
        {
            Real next_Real_Var = current_Real_Var * 2.0L;

            if (!(next_Real_Var > current_Real_Var) || next_Real_Var / 2.0L != current_Real_Var)
            {
                break;
            }

            current_Real_Var = next_Real_Var;
        }

        return current_Real_Var;
    }

    bool safeMultiplyReal(Real left_Real_Var, Real right_Real_Var, Real& result_Real_Var)
    {
        if (left_Real_Var == 0.0L || right_Real_Var == 0.0L)
        {
            result_Real_Var = 0.0L;
            return true;
        }

        if (absolute(left_Real_Var) > maximumReal() / absolute(right_Real_Var))
        {
            return false;
        }

        result_Real_Var = left_Real_Var * right_Real_Var;
        return true;
    }

    UInt maximumBinaryScale()
    {
        Real current_Real_Var = 1.0L;
        UInt scale_UInt_Var = 0;

        while (scale_UInt_Var < 100000)
        {
            Real next_Real_Var = current_Real_Var * 2.0L;

            if (!(next_Real_Var > current_Real_Var) || next_Real_Var / 2.0L != current_Real_Var)
            {
                break;
            }

            current_Real_Var = next_Real_Var;
            ++scale_UInt_Var;
        }

        return scale_UInt_Var;
    }
}
