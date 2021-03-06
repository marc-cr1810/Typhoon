#ifndef Ty_TOKEN_H
#define Ty_TOKEN_H

#include "../Port.h"

enum TokenType
{
	UNKNOWN_TOKEN = -1,
	START,
	END,
	NAME,
	NUMBER,
	FLOAT,
	STRING,
	BOOL,
	RETURN,
	OPERATOR,
	OPERATOR_SPECIAL,
	STATEMENT,
	DECLARATION,
	FUNCTION_CALL
};

enum OperatorType
{
	UNKNOWN = -1,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	LESS_THAN,
	GREATER_THAN,
	EQUAL,
	EQUAL_TO,
	NOT_EQUAL_TO,
	LESS_THAN_EQUAL_TO,
	GREATER_THAN_EQUAL_TO,
	ADD_EQUAL,
	SUBRTACT_EQUAL,
	MULTIPLY_EQUAL,
	DIVIDE_EQUAL,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	ASSIGN,
	SEMICOLON,
	COMMA
};

enum OperatorCategory
{
	CATEGORY_UNKNOWN = -1,
	ASSIGNMENT,
	CONDITIONAL,
	LOGICAL_OR,
	LOGICAL_AND,
	BITWISE_OR,
	BITWISE_AND,
	EQUALITY,
	RELATIONAL,
	SHIFT,
	ADDITIVE,
	MULTIPLICATIVE,
	UNARY,
	ASSIGNMENT_INLINE,
	POSTFIX
};

struct Token
{
    TokenType Type;
    Ty_string_t Value;

    Token(TokenType type, Ty_string_t value)
        : Type(type), Value(value)
    { }
};

struct OperatorToken
{
	TokenType Type;
	Ty_string_t Value;
	OperatorType OpType;
	OperatorCategory Category;

	OperatorToken(TokenType type, Ty_string_t value, OperatorType opType, OperatorCategory category)
		: Type(type), Value(value), OpType(opType), Category(category)
	{}
};

struct SpecialChar
{
	Ty_string_t format;
	Ty_string_t replacement;
};

static SpecialChar m_SpecialChars[3] = {
	{ "\\\\0", ""},
	{ "\\\\n", "\n" },
	{ "\\\\t", "	"}
};

#endif