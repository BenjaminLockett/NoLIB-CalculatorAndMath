#include "NoLib/Text/String.hpp"

namespace NoLib
{
    void String::allocate(ULong capacity_ULong_Var_New)
    {
        char* data_CharPtr_Var_New = new char[capacity_ULong_Var_New];

        for (ULong index_ULong_Var = 0; index_ULong_Var <= length_ULong_Var; ++index_ULong_Var)
        {
            data_CharPtr_Var_New[index_ULong_Var] = data_CharPtr_Var[index_ULong_Var];
        }

        delete[] data_CharPtr_Var;
        data_CharPtr_Var = data_CharPtr_Var_New;
        capacity_ULong_Var = capacity_ULong_Var_New;
    }

    String::String()
        : data_CharPtr_Var(new char[1]),
          length_ULong_Var(0),
          capacity_ULong_Var(1)
    {
        data_CharPtr_Var[0] = '\0';
    }

    String::String(const char* text_CharPtr_Var)
        : data_CharPtr_Var(new char[1]),
          length_ULong_Var(0),
          capacity_ULong_Var(1)
    {
        data_CharPtr_Var[0] = '\0';
        append(text_CharPtr_Var);
    }

    String::String(const String& other_String_Var)
        : data_CharPtr_Var(new char[other_String_Var.length_ULong_Var + 1]),
          length_ULong_Var(other_String_Var.length_ULong_Var),
          capacity_ULong_Var(other_String_Var.length_ULong_Var + 1)
    {
        for (ULong index_ULong_Var = 0; index_ULong_Var <= length_ULong_Var; ++index_ULong_Var)
        {
            data_CharPtr_Var[index_ULong_Var] = other_String_Var.data_CharPtr_Var[index_ULong_Var];
        }
    }

    String::String(String&& other_String_Var)
        : data_CharPtr_Var(other_String_Var.data_CharPtr_Var),
          length_ULong_Var(other_String_Var.length_ULong_Var),
          capacity_ULong_Var(other_String_Var.capacity_ULong_Var)
    {
        other_String_Var.data_CharPtr_Var = new char[1];
        other_String_Var.data_CharPtr_Var[0] = '\0';
        other_String_Var.length_ULong_Var = 0;
        other_String_Var.capacity_ULong_Var = 1;
    }

    String::~String()
    {
        delete[] data_CharPtr_Var;
    }

    String& String::operator=(const String& other_String_Var)
    {
        if (this == &other_String_Var)
        {
            return *this;
        }

        clear();
        reserve(other_String_Var.length_ULong_Var + 1);
        append(other_String_Var);
        return *this;
    }

    String& String::operator=(String&& other_String_Var)
    {
        if (this == &other_String_Var)
        {
            return *this;
        }

        delete[] data_CharPtr_Var;
        data_CharPtr_Var = other_String_Var.data_CharPtr_Var;
        length_ULong_Var = other_String_Var.length_ULong_Var;
        capacity_ULong_Var = other_String_Var.capacity_ULong_Var;

        other_String_Var.data_CharPtr_Var = new char[1];
        other_String_Var.data_CharPtr_Var[0] = '\0';
        other_String_Var.length_ULong_Var = 0;
        other_String_Var.capacity_ULong_Var = 1;
        return *this;
    }

    ULong String::length() const
    {
        return length_ULong_Var;
    }

    bool String::empty() const
    {
        return length_ULong_Var == 0;
    }

    const char* String::c_str() const
    {
        return data_CharPtr_Var;
    }

    char String::operator[](ULong index_ULong_Var) const
    {
        return index_ULong_Var < length_ULong_Var
            ? data_CharPtr_Var[index_ULong_Var]
            : '\0';
    }

    char& String::operator[](ULong index_ULong_Var)
    {
        return data_CharPtr_Var[index_ULong_Var];
    }

    void String::clear()
    {
        length_ULong_Var = 0;
        data_CharPtr_Var[0] = '\0';
    }

    void String::reserve(ULong capacity_ULong_Var_New)
    {
        if (capacity_ULong_Var_New > capacity_ULong_Var)
        {
            allocate(capacity_ULong_Var_New);
        }
    }

    bool String::pushBack(char character_Char_Var)
    {
        if (length_ULong_Var >= maximumUnsigned() - 1)
        {
            return false;
        }

        if (length_ULong_Var + 1 >= capacity_ULong_Var)
        {
            ULong capacity_ULong_Var_New = capacity_ULong_Var > maximumUnsigned() / 2
                ? maximumUnsigned()
                : capacity_ULong_Var * 2;

            if (capacity_ULong_Var_New <= length_ULong_Var + 1)
            {
                capacity_ULong_Var_New = length_ULong_Var + 2;
            }

            allocate(capacity_ULong_Var_New);
        }

        data_CharPtr_Var[length_ULong_Var] = character_Char_Var;
        ++length_ULong_Var;
        data_CharPtr_Var[length_ULong_Var] = '\0';
        return true;
    }

    bool String::popBack()
    {
        if (length_ULong_Var == 0)
        {
            return false;
        }

        --length_ULong_Var;
        data_CharPtr_Var[length_ULong_Var] = '\0';
        return true;
    }

    char String::back() const
    {
        return length_ULong_Var == 0 ? '\0' : data_CharPtr_Var[length_ULong_Var - 1];
    }

    bool String::append(const char* text_CharPtr_Var)
    {
        if (text_CharPtr_Var == nullptr)
        {
            return false;
        }

        ULong index_ULong_Var = 0;

        while (text_CharPtr_Var[index_ULong_Var] != '\0')
        {
            if (!pushBack(text_CharPtr_Var[index_ULong_Var]))
            {
                return false;
            }

            ++index_ULong_Var;
        }

        return true;
    }

    bool String::append(const String& text_String_Var)
    {
        if (this == &text_String_Var)
        {
            String copy_String_Var(text_String_Var);
            return append(copy_String_Var.c_str());
        }

        return append(text_String_Var.c_str());
    }

    String& String::operator+=(char character_Char_Var)
    {
        pushBack(character_Char_Var);
        return *this;
    }

    String& String::operator+=(const char* text_CharPtr_Var)
    {
        append(text_CharPtr_Var);
        return *this;
    }

    String& String::operator+=(const String& text_String_Var)
    {
        append(text_String_Var);
        return *this;
    }
}
