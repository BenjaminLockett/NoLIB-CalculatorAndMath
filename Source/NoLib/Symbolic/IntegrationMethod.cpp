#include "NoLib/Symbolic/IntegrationMethod.hpp"

namespace NoLib
{
    const char* rischIntegrationMethodName(RischIntegrationMethod method_RischIntegrationMethod_Var)
    {
        switch (method_RischIntegrationMethod_Var)
        {
        case RischElementaryRule:
            return "elementary rule and logarithmic-derivative matching";
        case RischRationalReduction:
            return "polynomial division and rational reduction";
        case RischPolynomialExponential:
            return "polynomial-exponential differential equation";
        case RischPolynomialTrigonometric:
            return "polynomial-trigonometric differential equations";
        case RischComposite:
            return "composite elementary methods";
        default:
            return "no supported elementary method";
        }
    }
}
