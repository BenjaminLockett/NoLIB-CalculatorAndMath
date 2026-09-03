#pragma once

// Generic stream operators for String; never included by the core.

#include "NoLib/Text/String.hpp"

namespace NoLib
{
    template <typename Stream_T>
    Stream_T& operator<<(Stream_T& output_Stream_Var, const String& text_String_Var)
    {
        output_Stream_Var << text_String_Var.c_str();
        return output_Stream_Var;
    }

    template <typename Stream_T>
    Stream_T& operator>>(Stream_T& input_Stream_Var, String& text_String_Var)
    {
        text_String_Var.clear();

        char character_Char_Var = '\0';
        input_Stream_Var >> character_Char_Var;

        if (!input_Stream_Var)
        {
            return input_Stream_Var;
        }

        text_String_Var.pushBack(character_Char_Var);

        while (true)
        {
            int next_Int_Var = input_Stream_Var.peek();

            if (next_Int_Var < 0 || next_Int_Var <= 32)
            {
                break;
            }

            input_Stream_Var.get(character_Char_Var);
            text_String_Var.pushBack(character_Char_Var);
        }

        return input_Stream_Var;
    }
}
