#pragma once

// Umbrella header. Existing code that wrote #include "NoLibMath.hpp" keeps
// compiling unchanged. New code should include only the modules it uses.
//
// Layering, lowest first. A header never includes anything above its own line.
//
//   Core -> Text -> Scalar -> Complex -> Polynomial -> Numerical
//        -> Symbolic representation -> Symbolic algorithms -> App

#include "NoLib/Core/Types.hpp"
#include "NoLib/Core/Utility.hpp"
#include "NoLib/Core/CheckedInteger.hpp"
#include "NoLib/Core/RealTraits.hpp"

#include "NoLib/Text/String.hpp"
#include "NoLib/Text/StringStreamAdapters.hpp"
#include "NoLib/Text/NumberParsing.hpp"
#include "NoLib/Text/IntegerFormatting.hpp"
#include "NoLib/Text/RealFormatting.hpp"

#include "NoLib/Scalar/ScientificNumber.hpp"
#include "NoLib/Scalar/SquareRoot.hpp"
#include "NoLib/Scalar/Constants.hpp"
#include "NoLib/Scalar/Logarithm.hpp"
#include "NoLib/Scalar/Exponential.hpp"
#include "NoLib/Scalar/Powers.hpp"
#include "NoLib/Scalar/Trigonometry.hpp"

#include "NoLib/Complex/ComplexNumber.hpp"
#include "NoLib/Complex/ComplexArithmetic.hpp"
#include "NoLib/Complex/ComplexElementary.hpp"
#include "NoLib/Complex/ComplexFormatting.hpp"

#include "NoLib/Polynomial/Polynomial.hpp"
#include "NoLib/Polynomial/PolynomialArithmetic.hpp"
#include "NoLib/Polynomial/PolynomialCalculus.hpp"
#include "NoLib/Polynomial/PolynomialRoots.hpp"

#include "NoLib/Numerical/Differentiation.hpp"
#include "NoLib/Numerical/Quadrature.hpp"
#include "NoLib/Numerical/Ode.hpp"

#include "NoLib/Symbolic/SymbolicType.hpp"
#include "NoLib/Symbolic/SymbolicNode.hpp"
#include "NoLib/Symbolic/SymbolicExpression.hpp"
#include "NoLib/Symbolic/SymbolicBuilder.hpp"
#include "NoLib/Symbolic/SymbolicCopy.hpp"
#include "NoLib/Symbolic/SymbolicComparison.hpp"
#include "NoLib/Symbolic/SymbolicSimplification.hpp"
#include "NoLib/Symbolic/SymbolicDifferentiation.hpp"
#include "NoLib/Symbolic/SymbolicParser.hpp"
#include "NoLib/Symbolic/SymbolicEvaluation.hpp"
#include "NoLib/Symbolic/SymbolicFormatting.hpp"
#include "NoLib/Symbolic/PolynomialBridge.hpp"
#include "NoLib/Symbolic/IntegrationMethod.hpp"
#include "NoLib/Symbolic/IntegrationCommon.hpp"
#include "NoLib/Symbolic/IntegrationRational.hpp"
#include "NoLib/Symbolic/IntegrationExtensions.hpp"
#include "NoLib/Symbolic/IntegrationVerification.hpp"
#include "NoLib/Symbolic/RischSubset.hpp"
