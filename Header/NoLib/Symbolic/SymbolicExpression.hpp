#pragma once

// Fixed-capacity append-only expression arena.

#include "NoLib/Symbolic/SymbolicNode.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    struct SymbolicExpression
    {
        SymbolicNode nodes_SymbolicNodeArray_Var[Capacity_UInt];
        UInt count_UInt_Var;
        UInt root_UInt_Var;

        SymbolicExpression()
            : count_UInt_Var(0),
              root_UInt_Var(invalidSymbolicNode())
        {
        }

        void clear()
        {
            count_UInt_Var = 0;
            root_UInt_Var = invalidSymbolicNode();
        }

        bool addNode(const SymbolicNode& node_SymbolicNode_Var, UInt& index_UInt_Var)
        {
            if (count_UInt_Var >= Capacity_UInt)
            {
                return false;
            }

            index_UInt_Var = count_UInt_Var;
            nodes_SymbolicNodeArray_Var[count_UInt_Var] =
                node_SymbolicNode_Var;
            ++count_UInt_Var;
            return true;
        }
    };
}
