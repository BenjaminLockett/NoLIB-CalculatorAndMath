#include "NoLib/Scalar/Trigonometry.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Scalar/Constants.hpp"

namespace NoLib
{
    Real reduceAngle(Real angle_Real_Var)
    {
        Real pi_Real_Var = piApproximation();
        Real fullTurn_Real_Var = 2.0L * pi_Real_Var;

        while (angle_Real_Var > pi_Real_Var)
        {
            angle_Real_Var -= fullTurn_Real_Var;
        }

        while (angle_Real_Var < -pi_Real_Var)
        {
            angle_Real_Var += fullTurn_Real_Var;
        }

        return angle_Real_Var;
    }

    Real sine(Real angle_Real_Var)
    {
        angle_Real_Var = reduceAngle(angle_Real_Var);
        Real term_Real_Var = angle_Real_Var;
        Real result_Real_Var = angle_Real_Var;

        for (UInt index_UInt_Var = 1; index_UInt_Var < 24; ++index_UInt_Var)
        {
            Real first_Real_Var =
                static_cast<Real>(2 * index_UInt_Var);
            Real second_Real_Var = first_Real_Var + 1.0L;
            term_Real_Var *=
                -angle_Real_Var * angle_Real_Var /
                (first_Real_Var * second_Real_Var);
            result_Real_Var += term_Real_Var;
        }

        return result_Real_Var;
    }

    Real cosine(Real angle_Real_Var)
    {
        angle_Real_Var = reduceAngle(angle_Real_Var);
        Real term_Real_Var = 1.0L;
        Real result_Real_Var = 1.0L;

        for (UInt index_UInt_Var = 1; index_UInt_Var < 24; ++index_UInt_Var)
        {
            Real first_Real_Var =
                static_cast<Real>(2 * index_UInt_Var - 1);
            Real second_Real_Var = first_Real_Var + 1.0L;
            term_Real_Var *=
                -angle_Real_Var * angle_Real_Var /
                (first_Real_Var * second_Real_Var);
            result_Real_Var += term_Real_Var;
        }

        return result_Real_Var;
    }

    Real arcTangent(Real value_Real_Var)
    {
        bool negative_Bool_Var = value_Real_Var < 0.0L;
        value_Real_Var = absolute(value_Real_Var);
        Real pi_Real_Var = piApproximation();
        Real offset_Real_Var = 0.0L;
        bool reciprocal_Bool_Var = false;

        if (value_Real_Var > 1.0L)
        {
            value_Real_Var = 1.0L / value_Real_Var;
            reciprocal_Bool_Var = true;
        }

        if (value_Real_Var > 0.4142135623730950488L)
        {
            value_Real_Var =
                (value_Real_Var - 1.0L) /
                (value_Real_Var + 1.0L);
            offset_Real_Var = pi_Real_Var / 4.0L;
        }

        Real squared_Real_Var = value_Real_Var * value_Real_Var;
        Real term_Real_Var = value_Real_Var;
        Real result_Real_Var = 0.0L;

        for (UInt index_UInt_Var = 0; index_UInt_Var < 64; ++index_UInt_Var)
        {
            Real piece_Real_Var =
                term_Real_Var /
                static_cast<Real>(2 * index_UInt_Var + 1);
            result_Real_Var +=
                (index_UInt_Var & 1U) == 0 ? piece_Real_Var : -piece_Real_Var;
            term_Real_Var *= squared_Real_Var;
        }

        result_Real_Var += offset_Real_Var;

        if (reciprocal_Bool_Var)
        {
            result_Real_Var = pi_Real_Var / 2.0L - result_Real_Var;
        }

        return negative_Bool_Var ? -result_Real_Var : result_Real_Var;
    }

    Real arcTangent2(Real y_Real_Var, Real x_Real_Var)
    {
        Real pi_Real_Var = piApproximation();

        if (x_Real_Var > 0.0L)
        {
            return arcTangent(y_Real_Var / x_Real_Var);
        }

        if (x_Real_Var < 0.0L && y_Real_Var >= 0.0L)
        {
            return arcTangent(y_Real_Var / x_Real_Var) + pi_Real_Var;
        }

        if (x_Real_Var < 0.0L && y_Real_Var < 0.0L)
        {
            return arcTangent(y_Real_Var / x_Real_Var) - pi_Real_Var;
        }

        if (y_Real_Var > 0.0L)
        {
            return pi_Real_Var / 2.0L;
        }

        if (y_Real_Var < 0.0L)
        {
            return -pi_Real_Var / 2.0L;
        }

        return 0.0L;
    }
}
