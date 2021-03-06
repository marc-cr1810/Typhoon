#ifndef Ty_GRAMMAR_H
#define Ty_GRAMMAR_H

#include "../Port.h"
#include "Token.h"
#include "Tokens.h"

enum GrammarType
{
	UNKNOWN_GRAMMAR = -1,
	CREATE_VAR,
	CREATE_FUNCTION,
	EXPRESSION_STATEMENT,
	IF_STATEMENT,
	ELSE_STATEMENT,
	ELSE_IF_STATEMENT,
	WHILE_STATEMENT,
	RETURN_FUNCTION
};

enum GrammarKeywordType
{
	UNKNOWN_KEYWORD = -1,
	KW_NAME,
	KW_EXPR
};

struct Grammar
{
	GrammarType Type;
	Ty_string_t Format;

	Grammar(GrammarType type, Ty_string_t format)
		: Type(type), Format(format)
	{}
};

const Ty_int32_t ExpressionTokenTypes[6] = {
	TokenType::NAME, 
	TokenType::NUMBER, 
	TokenType::FLOAT, 
	TokenType::STRING, 
	TokenType::BOOL,
	TokenType::OPERATOR
 };

const Grammar GrammarFormats[10] = {
	{ GrammarType::CREATE_VAR, "'var' NAME" },
	{ GrammarType::CREATE_VAR, "'var' NAME '=' EXPR" },
	{ GrammarType::CREATE_FUNCTION, "'func' NAME ARGS ':'" },
	{ GrammarType::EXPRESSION_STATEMENT, "EXPR" },
	{ GrammarType::IF_STATEMENT, "'if' EXPR ':'" },
	{ GrammarType::ELSE_IF_STATEMENT, "'else' 'if' EXPR ':'"},
	{ GrammarType::ELSE_STATEMENT, "'else' ':'" },
	{ GrammarType::WHILE_STATEMENT, "'while' EXPR ':'" },
	{ GrammarType::RETURN_FUNCTION, "'return'" },
	{ GrammarType::RETURN_FUNCTION, "'return' EXPR" }
};

static std::vector<Ty_string_t> GetGrammarKeywordList(Grammar grammar)
{
	Ty_string_t s = grammar.Format;
	std::smatch m;
	std::regex r("[^\\s\']+|\'([^\']*)\'");
	std::vector<Ty_string_t> result;

	while (std::regex_search(s, m, r))
	{
		result.push_back(m[0]);
		s = m.suffix().str();
	}
	return result;
}

static Grammar MatchGrammar(std::vector<Token> tokens)
{
	for (Grammar grammar : GrammarFormats)
	{
		int i = 0;
		std::vector<Ty_string_t> keywords = GetGrammarKeywordList(grammar);
		for (Ty_string_t keyword : keywords)
		{
			std::smatch m;
			if (std::regex_match(keyword, m, std::regex("^\'([^\']+)\'$")))
			{
				if (m[1] == tokens[i].Value)
				{
					i++;
					continue;
				}
				else goto NextFormat;
			}
			else
			{
				if (keyword == "NAME")
				{
					if (tokens[i].Type == TokenType::NAME)
						i++;
					else goto NextFormat;
				}
				else if (keyword == "EXPR")
				{
					bool isExpression = false;
					while (i < tokens.size() - 1)
					{
						for (Ty_int32_t exprTokenType : ExpressionTokenTypes)
						{
							if (exprTokenType == tokens[i].Type)
							{
								isExpression = true;
								i++;
								goto NextExprToken;
							}
						}
						if (!isExpression)
							goto NextFormat;
						else
							goto NextKeyword;
					NextExprToken: ;
					}
				}
				else if (keyword == "ARGS")
				{
					if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::LEFT_BRACKET)
					{
						int level = 1;
						i++;
						while (level > 0)
						{
							if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::LEFT_BRACKET)
							{
								level++;
								goto NextToken;
							}
							else if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::RIGHT_BRACKET)
							{
								level--;
								goto NextToken;
							}
							for (Ty_int32_t exprTokenType : ExpressionTokenTypes)
							{
								if (exprTokenType == tokens[i].Type)
									goto NextToken;
							}
							if (tokens[i].Type != TokenType::OPERATOR && tokens[i].Type != TokenType::DECLARATION)
								goto NextFormat;
						NextToken: ;
							i++;
						}
						if (level == 0)
							goto NextKeyword;
					}
					goto NextFormat;
				}
			}
		NextKeyword: ;
		}

		if (tokens[i].Type == TokenType::END)
			return { grammar.Type, grammar.Format };
	NextFormat: ;
	}
	return { GrammarType::UNKNOWN_GRAMMAR, "" };
}

#endif