#include "pch.h"
#include "syntax.h"

namespace curculator
{
	namespace syntax
	{
		Expression::Expression(eExpression expressionType)
		{
			this->expressionType = expressionType;
		}

		Literal::Literal(FixedReal val) : Expression(EXPRESSION_LITERAL)
		{
			this->val = val;
		}

		Parentheses::Parentheses(Expression *center) : Expression(EXPRESSION_PARENTHESES)
		{
			this->center = center;
		}

		Convert::Convert(Expression *center, unsigned long long sys) : Expression(EXPRESSION_CONVERT)
		{
			this->center = center;
			this->sys = sys;
		}

		UnaryExpression::UnaryExpression(Expression *center, eUnaryExpression unaryExpressionType) : Expression(EXPRESSION_UNARY)
		{
			this->center = center;
			this->unaryExpressionType = unaryExpressionType;
		}

		BinaryExpression::BinaryExpression(Expression *left, Expression *right, eBinaryExpression binaryExpressionType) : Expression(EXPRESSION_BINARY)
		{
			this->left = left;
			this->right = right;
			this->binaryExpressionType = binaryExpressionType;
		}
	}

	lexic::Token *Catch(List<lexic::Token*> *tokens, int pos)
	{
		if (pos >= 0 && pos < tokens->GetLength())
		{
			return tokens->Get(pos);
		}

		return NULL;
	}

	lexic::Token *Catch(List<lexic::Token*> *tokens, int pos, lexic::eToken tokenType)
	{
		lexic::Token *tok = Catch(tokens, pos);

		if (tok != NULL && tok->tokenType == tokenType) return tok;

		return NULL;
	}

	bool Match(lexic::Token *token, lexic::eToken tokenType)
	{
		return token != NULL && token->tokenType == tokenType;
	}

	bool Match(List<lexic::Token*> *tokens, int pos, lexic::eToken tokenType)
	{
		return Match(Catch(tokens, pos), tokenType);
	}

	lexic::eToken Get(lexic::Token *token)
	{
		if (token != NULL) return token->tokenType;

		return lexic::TOKEN_NAT;
	}

	lexic::eToken Get(List<lexic::Token*> *tokens, int pos)
	{
		return Get(Catch(tokens, pos));
	}

	syntax::Literal *ParseLiteral(List<lexic::Token*> *tokens, int *pos)
	{
		lexic::Literal *tok = (lexic::Literal*)Catch(tokens, *pos, lexic::TOKEN_LITERAL);

		if (tok != NULL)
		{
			(*pos)++;
			return new syntax::Literal(tok->val);
		}

		return NULL;
	}

	syntax::Expression *ParseExpressionsTree(List<lexic::Token*> *tokens, int *position, lexic::eToken endToken);

	syntax::Parentheses *ParseParentheses(List<lexic::Token*> *tokens, int *position)
	{
		int pos = *position;

		if (Match(tokens, pos, lexic::TOKEN_OPH))
		{
			syntax::Expression *exp = ParseExpressionsTree(tokens, &(++pos), lexic::TOKEN_CPH);

			if (Match(tokens, pos, lexic::TOKEN_CPH))
			{
				*position = ++pos;
				return new syntax::Parentheses(exp);
			}
		}

		return NULL;
	}

	syntax::Convert *ParseConvert(List<lexic::Token*> *tokens, int *pos)
	{
		switch (Get(tokens, *pos))
		{
		case lexic::TOKEN_CONV:
			(*pos)++;
			lexic::System *sys = (lexic::System*)Catch(tokens, *pos, lexic::TOKEN_SYSTEM);
			if (sys == NULL) return NULL;
			(*pos)++;
			return new syntax::Convert(NULL, sys->sys);
		}

		return NULL;
	}

	syntax::UnaryExpression *ParseUnaryExpression(List<lexic::Token*> *tokens, int *pos)
	{
		switch (Get(tokens, *pos))
		{
		case lexic::TOKEN_SUB:
			(*pos)++;
			return new syntax::UnaryExpression(NULL, syntax::UNARYEXPRESSION_NEG);
		}

		return NULL;
	}

	syntax::BinaryExpression *ParseBinaryExpression(List<lexic::Token*> *tokens, int *pos)
	{
		switch (Get(tokens, *pos))
		{
		case lexic::TOKEN_ADD:
			(*pos)++;
			return new syntax::BinaryExpression(NULL, NULL, syntax::BINARYEXPRESSION_ADD);
		case lexic::TOKEN_SUB:
			(*pos)++;
			return new syntax::BinaryExpression(NULL, NULL, syntax::BINARYEXPRESSION_SUB);
		case lexic::TOKEN_MUL:
			(*pos)++;
			return new syntax::BinaryExpression(NULL, NULL, syntax::BINARYEXPRESSION_MUL);
		case lexic::TOKEN_DIV:
			(*pos)++;
			return new syntax::BinaryExpression(NULL, NULL, syntax::BINARYEXPRESSION_DIV);
		case lexic::TOKEN_POW:
			(*pos)++;
			return new syntax::BinaryExpression(NULL, NULL, syntax::BINARYEXPRESSION_POW);
		}

		return NULL;
	}

	syntax::Expression *ParseExpression(List<lexic::Token*> *tokens, int *pos, bool isRight)
	{
		if (isRight)
		{
			syntax::Expression *exp = NULL;

			if ((exp = ParseLiteral(tokens, pos)) != NULL) return exp;
			else if ((exp = ParseParentheses(tokens, pos)) != NULL) return exp;
			else if ((exp = ParseUnaryExpression(tokens, pos)) != NULL) return exp;
		}
		else
		{
			syntax::Expression *exp = NULL;

			if ((exp = ParseBinaryExpression(tokens, pos)) != NULL) return exp;
			else if ((exp = ParseConvert(tokens, pos)) != NULL) return exp;
		}

		return NULL;
	}

	syntax::Expression **GetRightExpressionPointer(syntax::Expression *exp)
	{
		if (exp != NULL)
		{
			switch (exp->expressionType)
			{
			case syntax::EXPRESSION_UNARY:
				return &((syntax::UnaryExpression*)exp)->center;
			case syntax::EXPRESSION_BINARY:
				return &((syntax::BinaryExpression*)exp)->right;
			}
		}

		return NULL;
	}

	int GetExpressionPriority(syntax::Expression *exp)
	{
		if (exp != NULL)
		{
			switch (exp->expressionType)
			{
			case syntax::EXPRESSION_LITERAL:
			case syntax::EXPRESSION_PARENTHESES:
				return 1;
			case syntax::EXPRESSION_CONVERT:
				return 6;
			case syntax::EXPRESSION_UNARY:
				return 2;
			case syntax::EXPRESSION_BINARY:
				switch (((syntax::BinaryExpression*)exp)->binaryExpressionType)
				{
				case syntax::BINARYEXPRESSION_ADD:
				case syntax::BINARYEXPRESSION_SUB:
					return 5;
				case syntax::BINARYEXPRESSION_MUL:
				case syntax::BINARYEXPRESSION_DIV:
					return 4;
				case syntax::BINARYEXPRESSION_POW:
					return 3;
				}
				break;
			}
		}

		return 0;
	}

	syntax::Expression *ParseExpressionsTree(List<lexic::Token*> *tokens, int *position, lexic::eToken endToken)
	{
		int pos = *position;

		syntax::Expression *exp = ParseExpression(tokens, &pos, true);

		if (exp == NULL) return NULL;

		for (; pos < tokens->GetLength();)
		{
			if (Match(tokens, pos, endToken)) break;

			syntax::Expression **deepest = &exp;

			while (true)
			{
				syntax::Expression **ndeepest = GetRightExpressionPointer(*deepest);

				if (ndeepest != NULL)
				{
					deepest = ndeepest;
				}
				else break;
			}

			syntax::Expression *nexp = ParseExpression(tokens, &pos, (*deepest == NULL));

			if (nexp == NULL) return NULL;

			deepest = &exp;

			while (true)
			{
				if (*deepest != NULL)
				{
					if (GetExpressionPriority(nexp) < GetExpressionPriority(*deepest))
					{
						syntax::Expression **ndeepest = GetRightExpressionPointer(*deepest);

						if (ndeepest != NULL)
						{
							deepest = ndeepest;
							continue;
						}
					}
				}
				break;
			}

			switch (nexp->expressionType)
			{
			case syntax::EXPRESSION_UNARY:
				((syntax::UnaryExpression*)nexp)->center = *deepest;
				break;
			case syntax::EXPRESSION_BINARY:
				((syntax::BinaryExpression*)nexp)->left = *deepest;
				break;
			case syntax::EXPRESSION_CONVERT:
				((syntax::Convert*)nexp)->center = *deepest;
				break;
			}

			*deepest = nexp;
		}

		*position = pos;
		return exp;
	}

	Syntax::Syntax(Lexer *lexer)
	{
		List<lexic::Token*> *tokens = lexer->GetTokens();

		int pos = 0;

		exp = ParseExpressionsTree(tokens, &pos, lexic::TOKEN_NAT);
	}

	syntax::Expression *Syntax::GetExpression()
	{
		return exp;
	}
}