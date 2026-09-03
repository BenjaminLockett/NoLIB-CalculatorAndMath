#pragma once

// Small generic helpers used instead of <algorithm>.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    template <typename T>
    inline T absolute(const T& value_T_Var)
    {
        return value_T_Var < static_cast<T>(0)
            ? -value_T_Var
            : value_T_Var;
    }

    template <typename T>
    inline T minimum(const T& left_T_Var, const T& right_T_Var)
    {
        return left_T_Var < right_T_Var ? left_T_Var : right_T_Var;
    }

    template <typename T>
    inline T maximum(const T& left_T_Var, const T& right_T_Var)
    {
        return left_T_Var > right_T_Var ? left_T_Var : right_T_Var;
    }
}
