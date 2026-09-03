#include "NoLib/Scalar/Logarithm.hpp"

namespace NoLib
{
    bool naturalLog(Real value_Real_Var, Real& result_Real_Var)
    {
        if (value_Real_Var <= 0.0L)
        {
            return false;
        }

        const Real lnTwo_Real_Var =
            0.693147180559945309417232121458176568L;
        SLong scale_SLong_Var = 0;

        while (value_Real_Var > 1.5L)
        {
            value_Real_Var /= 2.0L;
            ++scale_SLong_Var;
        }

        while (value_Real_Var < 0.75L)
        {
            value_Real_Var *= 2.0L;
            --scale_SLong_Var;
        }

        Real z_Real_Var =
            (value_Real_Var - 1.0L) / (value_Real_Var + 1.0L);
        Real zSquared_Real_Var = z_Real_Var * z_Real_Var;
        Real term_Real_Var = z_Real_Var;
        Real sum_Real_Var = 0.0L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < 80; ++index_UInt_Var)
        {
            sum_Real_Var +=
                term_Real_Var / static_cast<Real>(2 * index_UInt_Var + 1);
            term_Real_Var *= zSquared_Real_Var;
        }

        result_Real_Var =
            2.0L * sum_Real_Var + scale_SLong_Var * lnTwo_Real_Var;
        return true;
    }
}
