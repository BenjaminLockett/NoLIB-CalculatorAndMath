#pragma once

// Names of the elementary methods the integrator can apply.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    enum RischIntegrationMethod
    {
        RischNoMethod,
        RischElementaryRule,
        RischRationalReduction,
        RischPolynomialExponential,
        RischPolynomialTrigonometric,
        RischComposite
    };

    const char* rischIntegrationMethodName(RischIntegrationMethod method_RischIntegrationMethod_Var);
}
