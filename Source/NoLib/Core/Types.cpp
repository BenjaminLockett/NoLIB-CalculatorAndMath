#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    ULong maximumUnsigned()
    {
        return ~0ULL;
    }

    SLong maximumSigned()
    {
        return static_cast<SLong>(maximumUnsigned() >> 1);
    }
}
