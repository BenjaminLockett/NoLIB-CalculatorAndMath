#pragma once

// Public orchestration of the restricted elementary integrator.

#include "NoLib/Symbolic/IntegrationExtensions.hpp"
#include "NoLib/Symbolic/IntegrationVerification.hpp"

namespace NoLib
{
    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt, UInt MaximumDegree_UInt>
    bool rischIntegrateSymbolicNode(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, UInt inputNode_UInt_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, UInt& outputNode_UInt_Var, RischIntegrationMethod& method_RischIntegrationMethod_Var)
    {
        SymbolicExpression<OutputCapacity_UInt> candidate_Symbolic_Var;

        if (rischStyleIntegrateSymbolicNode(input_Symbolic_Var, inputNode_UInt_Var, candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var) && copySymbolicNode(candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, outputNode_UInt_Var))
        {
            method_RischIntegrationMethod_Var = RischElementaryRule;
            return true;
        }

        if (inputNode_UInt_Var >= input_Symbolic_Var.count_UInt_Var)
        {
            return false;
        }

        const SymbolicNode& node_SymbolicNode_Var = input_Symbolic_Var.nodes_SymbolicNodeArray_Var[inputNode_UInt_Var];

        if (node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd || node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicSubtract)
        {
            UInt left_UInt_Var = invalidSymbolicNode();
            UInt right_UInt_Var = invalidSymbolicNode();
            RischIntegrationMethod leftMethod_RischIntegrationMethod_Var = RischNoMethod;
            RischIntegrationMethod rightMethod_RischIntegrationMethod_Var = RischNoMethod;

            if (!rischIntegrateSymbolicNode<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, node_SymbolicNode_Var.left_UInt_Var, output_Symbolic_Var, left_UInt_Var, leftMethod_RischIntegrationMethod_Var) || !rischIntegrateSymbolicNode<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, node_SymbolicNode_Var.right_UInt_Var, output_Symbolic_Var, right_UInt_Var, rightMethod_RischIntegrationMethod_Var))
            {
                return false;
            }

            bool combined_Bool_Var = node_SymbolicNode_Var.type_SymbolicType_Var == SymbolicAdd ? makeSymbolicAdd(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var) : makeSymbolicSubtract(output_Symbolic_Var, left_UInt_Var, right_UInt_Var, outputNode_UInt_Var);

            if (combined_Bool_Var)
            {
                method_RischIntegrationMethod_Var = leftMethod_RischIntegrationMethod_Var == rightMethod_RischIntegrationMethod_Var ? leftMethod_RischIntegrationMethod_Var : RischComposite;
            }

            return combined_Bool_Var;
        }

        candidate_Symbolic_Var.clear();

        if (tryRischRationalReduction<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, inputNode_UInt_Var, candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var) && copySymbolicNode(candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, outputNode_UInt_Var))
        {
            method_RischIntegrationMethod_Var = RischRationalReduction;
            return true;
        }

        candidate_Symbolic_Var.clear();
        RischIntegrationMethod extensionMethod_RischIntegrationMethod_Var = RischNoMethod;

        if (tryRischPolynomialElementaryExtension<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, inputNode_UInt_Var, candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var, extensionMethod_RischIntegrationMethod_Var) && copySymbolicNode(candidate_Symbolic_Var, candidate_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, outputNode_UInt_Var))
        {
            method_RischIntegrationMethod_Var = extensionMethod_RischIntegrationMethod_Var;
            return true;
        }

        return false;
    }

    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt, UInt MaximumDegree_UInt = 16>
    bool rischIntegrateSymbolic(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var, RischIntegrationMethod& method_RischIntegrationMethod_Var, bool& verified_Bool_Var)
    {
        output_Symbolic_Var.clear();
        method_RischIntegrationMethod_Var = RischNoMethod;
        verified_Bool_Var = false;

        if (!rischIntegrateSymbolicNode<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, input_Symbolic_Var.root_UInt_Var, output_Symbolic_Var, output_Symbolic_Var.root_UInt_Var, method_RischIntegrationMethod_Var))
        {
            output_Symbolic_Var.clear();
            method_RischIntegrationMethod_Var = RischNoMethod;
            return false;
        }

        verified_Bool_Var = verifySymbolicAntiderivative(input_Symbolic_Var, output_Symbolic_Var);
        return true;
    }

    template <UInt InputCapacity_UInt, UInt OutputCapacity_UInt, UInt MaximumDegree_UInt = 16>
    bool rischIntegrateSymbolic(const SymbolicExpression<InputCapacity_UInt>& input_Symbolic_Var, SymbolicExpression<OutputCapacity_UInt>& output_Symbolic_Var)
    {
        RischIntegrationMethod method_RischIntegrationMethod_Var = RischNoMethod;
        bool verified_Bool_Var = false;
        return rischIntegrateSymbolic<InputCapacity_UInt, OutputCapacity_UInt, MaximumDegree_UInt>(input_Symbolic_Var, output_Symbolic_Var, method_RischIntegrationMethod_Var, verified_Bool_Var);
    }
}
