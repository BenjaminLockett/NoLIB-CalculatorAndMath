#include "NoLib/Scalar/Constants.hpp"
#include "NoLib/Scalar/SquareRoot.hpp"

namespace NoLib
{
    Real piApproximation(UInt iterations_UInt_Var)
    {
        Real rootTwo_Real_Var = 0.0L;
        squareRoot(2.0L, rootTwo_Real_Var);

        Real a_Real_Var = 1.0L;
        Real b_Real_Var = 1.0L / rootTwo_Real_Var;
        Real t_Real_Var = 0.25L;
        Real p_Real_Var = 1.0L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < iterations_UInt_Var; ++index_UInt_Var)
        {
            Real nextA_Real_Var = (a_Real_Var + b_Real_Var) / 2.0L;
            Real nextB_Real_Var = 0.0L;
            squareRoot(a_Real_Var * b_Real_Var, nextB_Real_Var);
            Real difference_Real_Var = a_Real_Var - nextA_Real_Var;
            t_Real_Var -=
                p_Real_Var * difference_Real_Var * difference_Real_Var;
            a_Real_Var = nextA_Real_Var;
            b_Real_Var = nextB_Real_Var;
            p_Real_Var *= 2.0L;
        }

        Real sum_Real_Var = a_Real_Var + b_Real_Var;
        return sum_Real_Var * sum_Real_Var / (4.0L * t_Real_Var);
    }
}
