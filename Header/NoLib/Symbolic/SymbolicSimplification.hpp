#pragma once

// Constant factor extraction and scaling.

#include "NoLib/Symbolic/SymbolicBuilder.hpp"
#include "NoLib/Symbolic/SymbolicComparison.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    void extractSymbolicConstantFactor(const SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, UInt node_UInt_Var, Real& factor_Real_Var, UInt& core_UInt_Var)
    {
        Real constant_Real_Var = 0.0L;

        if (symbolicConstantValue(expression_Symbolic_Var, node_UInt_Var, constant_Real_Var))
        {
            factor_Real_Var = constant_Real_Var;
            core_UInt_Var = invalidSymbolicNode();
            return;
        }

        const SymbolicNode& node_SymbolicNode_Var =
            expression_Symbolic_Var
                .nodes_SymbolicNodeArray_Var[node_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicMultiply)
        {
            Real leftFactor_Real_Var = 1.0L;
            Real rightFactor_Real_Var = 1.0L;
            UInt leftCore_UInt_Var = invalidSymbolicNode();
            UInt rightCore_UInt_Var = invalidSymbolicNode();
            extractSymbolicConstantFactor(expression_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, leftFactor_Real_Var, leftCore_UInt_Var);
            extractSymbolicConstantFactor(expression_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, rightFactor_Real_Var, rightCore_UInt_Var);

            if (leftCore_UInt_Var == invalidSymbolicNode())
            {
                factor_Real_Var =
                    leftFactor_Real_Var * rightFactor_Real_Var;
                core_UInt_Var = rightCore_UInt_Var;
                return;
            }

            if (rightCore_UInt_Var == invalidSymbolicNode())
            {
                factor_Real_Var =
                    leftFactor_Real_Var * rightFactor_Real_Var;
                core_UInt_Var = leftCore_UInt_Var;
                return;
            }
        }

        factor_Real_Var = 1.0L;
        core_UInt_Var = node_UInt_Var;
    }

    template <UInt CandidateCapacity_UInt, UInt DerivativeCapacity_UInt>
    bool symbolicConstantRatio(const SymbolicExpression<CandidateCapacity_UInt>& candidate_Symbolic_Var, UInt candidateNode_UInt_Var, bool implicitOne_Bool_Var, const SymbolicExpression<DerivativeCapacity_UInt>& derivative_Symbolic_Var, UInt derivativeNode_UInt_Var, Real& ratio_Real_Var)
    {
        Real candidateFactor_Real_Var = 1.0L;
        UInt candidateCore_UInt_Var = invalidSymbolicNode();

        if (!implicitOne_Bool_Var)
        {
            extractSymbolicConstantFactor(candidate_Symbolic_Var, candidateNode_UInt_Var, candidateFactor_Real_Var, candidateCore_UInt_Var);
        }

        Real derivativeFactor_Real_Var = 1.0L;
        UInt derivativeCore_UInt_Var = invalidSymbolicNode();
        extractSymbolicConstantFactor(derivative_Symbolic_Var, derivativeNode_UInt_Var, derivativeFactor_Real_Var, derivativeCore_UInt_Var);

        if (derivativeFactor_Real_Var == 0.0L)
        {
            return false;
        }

        if (candidateCore_UInt_Var == invalidSymbolicNode() && derivativeCore_UInt_Var == invalidSymbolicNode())
        {
            ratio_Real_Var =
                candidateFactor_Real_Var / derivativeFactor_Real_Var;
            return true;
        }

        if (candidateCore_UInt_Var == invalidSymbolicNode() || derivativeCore_UInt_Var == invalidSymbolicNode() || !sameSymbolicNode(candidate_Symbolic_Var, candidateCore_UInt_Var, derivative_Symbolic_Var, derivativeCore_UInt_Var))
        {
            return false;
        }

        ratio_Real_Var =
            candidateFactor_Real_Var / derivativeFactor_Real_Var;
        return true;
    }

    template <UInt Capacity_UInt>
    bool scaleSymbolicNode(SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var, Real scale_Real_Var, UInt node_UInt_Var, UInt& result_UInt_Var)
    {
        UInt scaleNode_UInt_Var = invalidSymbolicNode();
        return makeSymbolicConstant(expression_Symbolic_Var, scale_Real_Var, scaleNode_UInt_Var) &&
               makeSymbolicMultiply(expression_Symbolic_Var, scaleNode_UInt_Var, node_UInt_Var, result_UInt_Var);
    }
}
