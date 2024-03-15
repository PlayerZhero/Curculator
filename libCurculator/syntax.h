#pragma once

#include "lexic.h"

namespace curculator
{
	namespace syntax
	{
		enum eExpression : unsigned char
		{
			EXPRESSION_LITERAL = 0,
			EXPRESSION_PARENTHESES,
			EXPRESSION_CONVERT,

			EXPRESSION_UNARY = 5,
			EXPRESSION_BINARY
		};

		struct Expression
		{
		public:
			eExpression expressionType;

			Expression(eExpression expressionType);
		};

		struct Literal : public Expression
		{
		public:
			FixedReal val;

			Literal(FixedReal val);
		};

		struct Parentheses : public Expression
		{
		public:
			Expression *center;

			Parentheses(Expression *center);
		};

		struct Convert : public Expression
		{
		public:
			Expression *center;
			unsigned long long sys;

			Convert(Expression *center, unsigned long long sys);
		};

		enum eUnaryExpression : unsigned char
		{
			UNARYEXPRESSION_NEG = 0
		};

		struct UnaryExpression : public Expression
		{
		public:
			Expression *center;
			eUnaryExpression unaryExpressionType;

			UnaryExpression(Expression *center, eUnaryExpression unaryExpressionType);
		};

		enum eBinaryExpression : unsigned char
		{
			BINARYEXPRESSION_ADD = 0,
			BINARYEXPRESSION_SUB,
			BINARYEXPRESSION_MUL,
			BINARYEXPRESSION_DIV,
			BINARYEXPRESSION_POW
		};

		struct BinaryExpression : public Expression
		{
		public:
			Expression *left;
			Expression *right;
			eBinaryExpression binaryExpressionType;

			BinaryExpression(Expression *left, Expression *right, eBinaryExpression binaryExpressionType);
		};
	}

	class Syntax
	{
	private:
		syntax::Expression *exp;

	public:
		Syntax(Lexer *lexer);

		syntax::Expression *GetExpression();
	};
}