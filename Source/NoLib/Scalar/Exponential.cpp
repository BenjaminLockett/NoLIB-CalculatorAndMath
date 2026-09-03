#include "NoLib/Scalar/Exponential.hpp"
#include "NoLib/Core/RealTraits.hpp"

namespace NoLib
{
    bool exponential(Real value_Real_Var, Real& result_Real_Var)
    {
        const Real lnTwo_Real_Var =
            0.693147180559945309417232121458176568L;
        UInt maximumScale_UInt_Var = maximumBinaryScale();
        Real maximumInput_Real_Var =
            static_cast<Real>(maximumScale_UInt_Var) * lnTwo_Real_Var;

        if (value_Real_Var > maximumInput_Real_Var)
        {
            return false;
        }

        if (value_Real_Var < -maximumInput_Real_Var * 2.0L)
        {
            result_Real_Var = 0.0L;
            return true;
        }

        Real rawScale_Real_Var = value_Real_Var / lnTwo_Real_Var;
        SLong scale_SLong_Var = rawScale_Real_Var >= 0.0L
            ? static_cast<SLong>(rawScale_Real_Var + 0.5L)
            : static_cast<SLong>(rawScale_Real_Var - 0.5L);
        Real remainder_Real_Var =
            value_Real_Var - scale_SLong_Var * lnTwo_Real_Var;
        Real term_Real_Var = 1.0L;
        Real sum_Real_Var = 1.0L;

        for (UInt index_UInt_Var = 1; index_UInt_Var <= 64; ++index_UInt_Var)
        {
            term_Real_Var *=
                remainder_Real_Var / static_cast<Real>(index_UInt_Var);
            sum_Real_Var += term_Real_Var;
        }

        while (scale_SLong_Var > 0)
        {
            Real next_Real_Var = sum_Real_Var * 2.0L;

            if (!(next_Real_Var > sum_Real_Var) || next_Real_Var / 2.0L != sum_Real_Var)
            {
                return false;
            }

            sum_Real_Var = next_Real_Var;
            --scale_SLong_Var;
        }

        while (scale_SLong_Var < 0)
        {
            sum_Real_Var /= 2.0L;
            ++scale_SLong_Var;
        }

        result_Real_Var = sum_Real_Var;
        return true;
    }
}
