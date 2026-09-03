#include "NoLib/Scalar/SquareRoot.hpp"
#include "NoLib/Core/Utility.hpp"
#include <iostream>

namespace NoLib
{
    bool squareRoot(Real value_Real_Var, Real& result_Real_Var, UInt iterations_UInt_Var)
    {
        if (value_Real_Var < 0.0L)
        {
            return false;
        }

        if (value_Real_Var == 0.0L)
        {
            result_Real_Var = 0.0L;
            return true;
        }

        Real guess_Real_Var = value_Real_Var >= 1.0L ? value_Real_Var / 2.0L + 1.0L : 1.0L;

        for (UInt i = 0; i < iterations_UInt_Var; ++i)
        {
            Real next_Real_Var =
                0.5L * (guess_Real_Var + value_Real_Var / guess_Real_Var);
            Real scale_Real_Var = maximum(1.0L, absolute(next_Real_Var));

            if (absolute(next_Real_Var - guess_Real_Var) <= 1.0e-18L * scale_Real_Var)
            {
                guess_Real_Var = next_Real_Var;
				std::cerr << "Converged after " << guess_Real_Var << " iterations." << std::endl;
                break;
            }

            guess_Real_Var = next_Real_Var;
        }

        result_Real_Var = guess_Real_Var;
        return true;
    }
}
