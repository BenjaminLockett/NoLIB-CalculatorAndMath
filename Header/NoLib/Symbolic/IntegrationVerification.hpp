#pragma once

// Sampled validation of a proposed antiderivative.

#include "NoLib/Symbolic/SymbolicDifferentiation.hpp"
#include "NoLib/Symbolic/SymbolicEvaluation.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt IntegralCapacity_UInt>
    bool verifySymbolicAntiderivative(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, const SymbolicExpression<IntegralCapacity_UInt>& integral_Symbolic_Var)
    {
        SymbolicExpression<IntegralCapacity_UInt * 3U> derivative_Symbolic_Var;

        if (!differentiateSymbolic(integral_Symbolic_Var, derivative_Symbolic_Var))
        {
            return false;
        }

        const Real samples_RealArray_Var[11] = {-3.25L, -2.0L, -1.25L, -0.5L, -0.125L, 0.0L, 0.25L, 0.75L, 1.5L, 2.25L, 3.5L};
        UInt checked_UInt_Var = 0;

        for (UInt index_UInt_Var = 0; index_UInt_Var < 11; ++index_UInt_Var)
        {
            Real expected_Real_Var = 0.0L;
            Real actual_Real_Var = 0.0L;

            if (!evaluateSymbolic(input_Symbolic_Var, samples_RealArray_Var[index_UInt_Var], expected_Real_Var) || !evaluateSymbolic(derivative_Symbolic_Var, samples_RealArray_Var[index_UInt_Var], actual_Real_Var))
            {
                continue;
            }

            if (expected_Real_Var != expected_Real_Var || actual_Real_Var != actual_Real_Var || expected_Real_Var * 0.0L != 0.0L || actual_Real_Var * 0.0L != 0.0L)
            {
                continue;
            }

            Real scale_Real_Var = maximum(1.0L, maximum(absolute(expected_Real_Var), absolute(actual_Real_Var)));

            if (absolute(expected_Real_Var - actual_Real_Var) > 2.0e-8L * scale_Real_Var)
            {
                return false;
            }

            ++checked_UInt_Var;
        }

        return checked_UInt_Var >= 4;
    }
}
