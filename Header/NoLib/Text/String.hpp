#pragma once

// Small owning string used instead of <string>.

#include "NoLib/Core/Types.hpp"

namespace NoLib
{
    class String
    {
    private:
        char* data_CharPtr_Var;
        ULong length_ULong_Var;
        ULong capacity_ULong_Var;

        // Grows the buffer and copies the terminator with the characters.
        void allocate(ULong capacity_ULong_Var_New);

    public:
        String();
        String(const char* text_CharPtr_Var);
        String(const String& other_String_Var);
        String(String&& other_String_Var);
        ~String();

        String& operator=(const String& other_String_Var);
        String& operator=(String&& other_String_Var);

        ULong length() const;
        bool empty() const;
        const char* c_str() const;

        char operator[](ULong index_ULong_Var) const;
        char& operator[](ULong index_ULong_Var);

        void clear();
        void reserve(ULong capacity_ULong_Var_New);

        // Returns false only when the character cannot be stored.
        bool pushBack(char character_Char_Var);
        bool popBack();
        char back() const;

        bool append(const char* text_CharPtr_Var);
        bool append(const String& text_String_Var);

        String& operator+=(char character_Char_Var);
        String& operator+=(const char* text_CharPtr_Var);
        String& operator+=(const String& text_String_Var);
    };
}
