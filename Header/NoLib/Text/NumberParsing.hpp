#pragma once

// Text to number conversion without <sstream>.

#include "NoLib/Core/Types.hpp"
#include "NoLib/Core/CheckedInteger.hpp"
#include "NoLib/Text/String.hpp"

namespace NoLib
{
    bool readUnsigned(const String& text_String_Var, ULong& result_ULong_Var);

    bool readSigned(const String& text_String_Var, SLong& result_SLong_Var);

    bool readReal(const String& text_String_Var, Real& result_Real_Var);
}
