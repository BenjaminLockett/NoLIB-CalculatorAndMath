#pragma once

// Fundamental numeric aliases used by every other module.

namespace NoLib
{
    using UInt = unsigned int;          // Small unsigned whole number.
    using ULong = unsigned long long;   // Large unsigned whole number.
    using SLong = long long;            // Signed whole number.
    using Real = long double;            // Approximate real number.
    ULong maximumUnsigned();

    SLong maximumSigned();
}
