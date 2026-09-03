#pragma once

// Tokenisation and recursive-descent parsing.

#include "NoLib/Symbolic/SymbolicBuilder.hpp"
#include "NoLib/Text/String.hpp"
#include "NoLib/Text/NumberParsing.hpp"

namespace NoLib
{
    template <UInt Capacity_UInt>
    class SymbolicParser
    {
    private:
        const String& text_String_Var;
        SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var;
        ULong position_ULong_Var;
        bool failed_Bool_Var;

        void skipSpaces()
        {
            while (position_ULong_Var < text_String_Var.length() && (text_String_Var[position_ULong_Var] == ' ' || text_String_Var[position_ULong_Var] == '\t'))
            {
                ++position_ULong_Var;
            }
        }

        bool consume(char character_Char_Var)
        {
            skipSpaces();

            if (position_ULong_Var < text_String_Var.length() && text_String_Var[position_ULong_Var] == character_Char_Var)
            {
                ++position_ULong_Var;
                return true;
            }

            return false;
        }

        bool matchName(const char* name_CharPtr_Var)
        {
            skipSpaces();
            ULong length_ULong_Var = 0;

            while (name_CharPtr_Var[length_ULong_Var] != '\0')
            {
                if (position_ULong_Var + length_ULong_Var >= text_String_Var.length() || text_String_Var[position_ULong_Var + length_ULong_Var] != name_CharPtr_Var[length_ULong_Var])
                {
                    return false;
                }

                ++length_ULong_Var;
            }

            position_ULong_Var += length_ULong_Var;
            return true;
        }

        bool startsImplicitFactor()
        {
            skipSpaces();

            if (position_ULong_Var >= text_String_Var.length())
            {
                return false;
            }

            char character_Char_Var = text_String_Var[position_ULong_Var];
            return character_Char_Var == '(' || character_Char_Var == 'x' || character_Char_Var == '.' || (character_Char_Var >= '0' && character_Char_Var <= '9') || character_Char_Var == 'e' || character_Char_Var == 'l' || character_Char_Var == 's' || character_Char_Var == 'c' || character_Char_Var == 'a';
        }

        bool parseNumber(UInt& result_UInt_Var)
        {
            skipSpaces();
            ULong start_ULong_Var = position_ULong_Var;
            bool digit_Bool_Var = false;
            bool point_Bool_Var = false;

            while (position_ULong_Var < text_String_Var.length())
            {
                char character_Char_Var =
                    text_String_Var[position_ULong_Var];

                if (character_Char_Var >= '0' && character_Char_Var <= '9')
                {
                    digit_Bool_Var = true;
                    ++position_ULong_Var;
                    continue;
                }

                if (character_Char_Var == '.' && !point_Bool_Var)
                {
                    point_Bool_Var = true;
                    ++position_ULong_Var;
                    continue;
                }

                break;
            }

            if (!digit_Bool_Var)
            {
                position_ULong_Var = start_ULong_Var;
                return false;
            }

            if (position_ULong_Var < text_String_Var.length() && (text_String_Var[position_ULong_Var] == 'e' || text_String_Var[position_ULong_Var] == 'E'))
            {
                ++position_ULong_Var;

                if (position_ULong_Var < text_String_Var.length() && (text_String_Var[position_ULong_Var] == '+' || text_String_Var[position_ULong_Var] == '-'))
                {
                    ++position_ULong_Var;
                }

                ULong exponentStart_ULong_Var = position_ULong_Var;

                while (position_ULong_Var < text_String_Var.length() && text_String_Var[position_ULong_Var] >= '0' && text_String_Var[position_ULong_Var] <= '9')
                {
                    ++position_ULong_Var;
                }

                if (position_ULong_Var == exponentStart_ULong_Var)
                {
                    failed_Bool_Var = true;
                    return false;
                }
            }

            String number_String_Var;

            for (ULong index_ULong_Var = start_ULong_Var; index_ULong_Var < position_ULong_Var; ++index_ULong_Var)
            {
                number_String_Var += text_String_Var[index_ULong_Var];
            }

            Real number_Real_Var = 0.0L;

            if (!readReal(number_String_Var, number_Real_Var) || !makeSymbolicConstant(expression_Symbolic_Var, number_Real_Var, result_UInt_Var))
            {
                failed_Bool_Var = true;
                return false;
            }

            return true;
        }

        bool parseSignedExponent(SLong& exponent_SLong_Var)
        {
            skipSpaces();
            String exponent_String_Var;

            if (position_ULong_Var < text_String_Var.length() && (text_String_Var[position_ULong_Var] == '+' || text_String_Var[position_ULong_Var] == '-'))
            {
                exponent_String_Var +=
                    text_String_Var[position_ULong_Var];
                ++position_ULong_Var;
            }

            ULong digitStart_ULong_Var = position_ULong_Var;

            while (position_ULong_Var < text_String_Var.length() && text_String_Var[position_ULong_Var] >= '0' && text_String_Var[position_ULong_Var] <= '9')
            {
                exponent_String_Var +=
                    text_String_Var[position_ULong_Var];
                ++position_ULong_Var;
            }

            return position_ULong_Var != digitStart_ULong_Var &&
                   readSigned(exponent_String_Var, exponent_SLong_Var);
        }

        bool parsePrimary(UInt& result_UInt_Var)
        {
            skipSpaces();

            if (consume('('))
            {
                if (!parseExpression(result_UInt_Var) || !consume(')'))
                {
                    failed_Bool_Var = true;
                    return false;
                }

                return true;
            }

            if (position_ULong_Var < text_String_Var.length() && text_String_Var[position_ULong_Var] == 'x')
            {
                ++position_ULong_Var;
                return makeSymbolicVariable(expression_Symbolic_Var, result_UInt_Var);
            }

            ULong savedPosition_ULong_Var = position_ULong_Var;
            SymbolicType functionType_SymbolicType_Var = SymbolicConstant;
            bool function_Bool_Var = false;

            if (matchName("lnabs"))
            {
                functionType_SymbolicType_Var = SymbolicLogAbsolute;
                function_Bool_Var = true;
            }
            else
            {
                position_ULong_Var = savedPosition_ULong_Var;

                if (matchName("exp"))
                {
                    functionType_SymbolicType_Var = SymbolicExponential;
                    function_Bool_Var = true;
                }
                else
                {
                    position_ULong_Var = savedPosition_ULong_Var;

                    if (matchName("ln"))
                    {
                        functionType_SymbolicType_Var = SymbolicLogAbsolute;
                        function_Bool_Var = true;
                    }
                    else
                    {
                        position_ULong_Var = savedPosition_ULong_Var;

                        if (matchName("sin"))
                        {
                            functionType_SymbolicType_Var = SymbolicSine;
                            function_Bool_Var = true;
                        }
                        else
                        {
                            position_ULong_Var = savedPosition_ULong_Var;

                            if (matchName("cos"))
                            {
                                functionType_SymbolicType_Var = SymbolicCosine;
                                function_Bool_Var = true;
                            }
                            else
                            {
                                position_ULong_Var = savedPosition_ULong_Var;

                                if (matchName("atan"))
                                {
                                    functionType_SymbolicType_Var = SymbolicArcTangent;
                                    function_Bool_Var = true;
                                }
                            }
                        }
                    }
                }
            }

            if (function_Bool_Var)
            {
                UInt argument_UInt_Var = invalidSymbolicNode();

                if (!consume('(') || !parseExpression(argument_UInt_Var) || !consume(')') || !makeSymbolicUnary(expression_Symbolic_Var, functionType_SymbolicType_Var, argument_UInt_Var, result_UInt_Var))
                {
                    failed_Bool_Var = true;
                    return false;
                }

                return true;
            }

            position_ULong_Var = savedPosition_ULong_Var;
            return parseNumber(result_UInt_Var);
        }

        bool parsePower(UInt& result_UInt_Var)
        {
            if (!parsePrimary(result_UInt_Var))
            {
                return false;
            }

            if (consume('^'))
            {
                SLong exponent_SLong_Var = 0;

                if (!parseSignedExponent(exponent_SLong_Var) || !makeSymbolicIntegerPower(expression_Symbolic_Var, result_UInt_Var, exponent_SLong_Var, result_UInt_Var))
                {
                    failed_Bool_Var = true;
                    return false;
                }
            }

            return true;
        }

        bool parseUnary(UInt& result_UInt_Var)
        {
            if (consume('+'))
            {
                return parseUnary(result_UInt_Var);
            }

            if (consume('-'))
            {
                UInt argument_UInt_Var = invalidSymbolicNode();
                UInt negativeOne_UInt_Var = invalidSymbolicNode();

                return parseUnary(argument_UInt_Var) &&
                       makeSymbolicConstant(expression_Symbolic_Var, -1.0L, negativeOne_UInt_Var) &&
                       makeSymbolicMultiply(expression_Symbolic_Var, negativeOne_UInt_Var, argument_UInt_Var, result_UInt_Var);
            }

            return parsePower(result_UInt_Var);
        }

        bool parseTerm(UInt& result_UInt_Var)
        {
            if (!parseUnary(result_UInt_Var))
            {
                return false;
            }

            while (true)
            {
                if (consume('*'))
                {
                    UInt right_UInt_Var = invalidSymbolicNode();

                    if (!parseUnary(right_UInt_Var) || !makeSymbolicMultiply(expression_Symbolic_Var, result_UInt_Var, right_UInt_Var, result_UInt_Var))
                    {
                        failed_Bool_Var = true;
                        return false;
                    }
                }
                else if (consume('/'))
                {
                    UInt right_UInt_Var = invalidSymbolicNode();

                    if (!parseUnary(right_UInt_Var) || !makeSymbolicDivide(expression_Symbolic_Var, result_UInt_Var, right_UInt_Var, result_UInt_Var))
                    {
                        failed_Bool_Var = true;
                        return false;
                    }
                }
                else if (startsImplicitFactor())
                {
                    UInt right_UInt_Var = invalidSymbolicNode();

                    if (!parseUnary(right_UInt_Var) || !makeSymbolicMultiply(expression_Symbolic_Var, result_UInt_Var, right_UInt_Var, result_UInt_Var))
                    {
                        failed_Bool_Var = true;
                        return false;
                    }
                }
                else
                {
                    break;
                }
            }

            return true;
        }

        bool parseExpression(UInt& result_UInt_Var)
        {
            if (!parseTerm(result_UInt_Var))
            {
                return false;
            }

            while (true)
            {
                if (consume('+'))
                {
                    UInt right_UInt_Var = invalidSymbolicNode();

                    if (!parseTerm(right_UInt_Var) || !makeSymbolicAdd(expression_Symbolic_Var, result_UInt_Var, right_UInt_Var, result_UInt_Var))
                    {
                        failed_Bool_Var = true;
                        return false;
                    }
                }
                else if (consume('-'))
                {
                    UInt right_UInt_Var = invalidSymbolicNode();

                    if (!parseTerm(right_UInt_Var) || !makeSymbolicSubtract(expression_Symbolic_Var, result_UInt_Var, right_UInt_Var, result_UInt_Var))
                    {
                        failed_Bool_Var = true;
                        return false;
                    }
                }
                else
                {
                    break;
                }
            }

            return true;
        }

    public:
        SymbolicParser(const String& text_String_Var_New, SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var_New)
            : text_String_Var(text_String_Var_New),
              expression_Symbolic_Var(expression_Symbolic_Var_New),
              position_ULong_Var(0),
              failed_Bool_Var(false)
        {
        }

        bool parse()
        {
            expression_Symbolic_Var.clear();

            if (!parseExpression(expression_Symbolic_Var.root_UInt_Var))
            {
                expression_Symbolic_Var.clear();
                return false;
            }

            skipSpaces();

            if (failed_Bool_Var || position_ULong_Var != text_String_Var.length())
            {
                expression_Symbolic_Var.clear();
                return false;
            }

            return true;
        }
    };

    template <UInt Capacity_UInt>
    bool parseSymbolicExpression(const String& text_String_Var, SymbolicExpression<Capacity_UInt>& expression_Symbolic_Var)
    {
        SymbolicParser<Capacity_UInt> parser_SymbolicParser_Var(text_String_Var, expression_Symbolic_Var);
        return parser_SymbolicParser_Var.parse();
    }
}
