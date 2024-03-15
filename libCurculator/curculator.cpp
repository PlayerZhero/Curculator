#include "pch.h"
#include "curculator.h"

#include "fixedreal.h"

#include <combaseapi.h>

#include "lexic.h"
#include "syntax.h"

#include <Windows.h>
#include <stdio.h>

using namespace curculator;

FixedReal calculate(syntax::Expression *exp)
{
	switch (exp->expressionType)
	{
	case syntax::EXPRESSION_LITERAL:
		return ((syntax::Literal*)exp)->val;
	case syntax::EXPRESSION_PARENTHESES:
		return calculate(((syntax::Parentheses*)exp)->center);
	case syntax::EXPRESSION_CONVERT:
		return calculate(((syntax::Convert*)exp)->center);
	case syntax::EXPRESSION_UNARY:
	{
		FixedReal c = calculate(((syntax::UnaryExpression*)exp)->center);

		switch (((syntax::UnaryExpression*)exp)->unaryExpressionType)
		{
		case syntax::UNARYEXPRESSION_NEG:
			c.SetNegative(!c.IsNegative());
			return c;
		}
	}
	break;
	case syntax::EXPRESSION_BINARY:
	{
		FixedReal l = calculate(((syntax::BinaryExpression*)exp)->left);
		FixedReal r = calculate(((syntax::BinaryExpression*)exp)->right);

		switch (((syntax::BinaryExpression*)exp)->binaryExpressionType)
		{
		case syntax::BINARYEXPRESSION_ADD: return FixedReal::Add(l, r);
		case syntax::BINARYEXPRESSION_SUB: return FixedReal::Sub(l, r);
		case syntax::BINARYEXPRESSION_MUL: return FixedReal::Mul(l, r);
		case syntax::BINARYEXPRESSION_DIV: return FixedReal::Div(l, r);
		case syntax::BINARYEXPRESSION_POW: return FixedReal::Pow(l, r);
		}
	}
	break;
	}

	return FixedReal(false, 0, 0);
}

wchar_t *calculatetotext(syntax::Expression *exp)
{
	unsigned long long sys = 10;

	syntax::Expression *e = exp;

	if (e->expressionType == syntax::EXPRESSION_CONVERT)
	{
		sys = ((syntax::Convert*)e)->sys;

		e = ((syntax::Convert*)e)->center;
	}

	wchar_t *chrs = calculate(e).Print(sys);

	wchar_t *output = NULL;

	if (sys == 10)
	{
		output = chrs;
	}
	else
	{
		wchar_t s[10];
		int pos = 0;

		for (int i = sys; i > 0; i /= 10)
		{
			s[pos] = (i % 10) + L'\x2080';
			pos++;
		}

		int chrslen = wcslen(chrs);
		output = new wchar_t[chrslen + pos + 1];

		memcpy(output, chrs, chrslen * sizeof(wchar_t));
		for (int i = pos - 1; i >= 0; i--)
		{
			output[pos - i + chrslen - 1] = s[i];
		}
		output[chrslen + pos] = L'\0';

		delete[] chrs;
	}

	return output;
}

wchar_t *__stdcall calculate(wchar_t *input)
{
	//FILE *stdo = stdout;
	//if (AllocConsole())
	//{
	//	freopen_s(&stdo, "CONOUT$", "w", stdout);
	//	//freopen("CONOUT$", "w", stdout);
	//	SetConsoleTitle(L"Debug");
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	//}
	//printf("Hello world!\n");

	FixedReal fr = FixedReal(false, 0b0101, 0b01010101010101010101010000000000);

	//FixedReal fr1 = FixedReal(false, 0b0111,	0b11111111111111111111110000000000);
	//FixedReal fr2 = FixedReal(false, 0b0,		0b00000000000000000000010000000000);

	//fr = FixedReal::LShift(fr, 2);
	//fr = FixedReal::RShift(fr, 2);

	//fr = FixedReal::Add(fr, FixedReal::RShift(fr, 1));

	//fr = FixedReal::Add(fr1, fr2);

	//FixedReal fr1 = FixedReal(false, 0b111,		0b11111111111111111111110000000000);
	//FixedReal fr2 = FixedReal(false, 0b10,		0b10000000000000000000000000000000);

	//fr = FixedReal::Mul(fr1, fr2);
	//fr = FixedReal::Add(fr1, fr2);
	//fr = FixedReal::Add(FixedReal::LShift(fr1, 0), FixedReal::LShift(fr1, 2));
	//fr = FixedReal::Add(FixedReal::LShift(fr2, 0), FixedReal::RShift(fr2, 2));

	//FixedReal fr1 = FixedReal(false, 0b1000,		0b0);
	//FixedReal fr2 = FixedReal(false, 0b0010,		0b0);

	FixedReal fr1 = FixedReal(false, 9, 0b0);
	FixedReal fr2 = FixedReal(false, 3, 0b0);

	//fr = FixedReal::Div(fr1, fr2);

	//FixedReal f = FixedReal::Pow(FixedReal(false, 16, 0), FixedReal(false, 0, (unsigned long long)1 << 62));

	Lexer *lexer = new Lexer(input);

	Syntax *syntax = new Syntax(lexer);

	syntax::Expression *exp = syntax->GetExpression();

	wchar_t *res = /*fr.Print()*/NULL;

	if (exp != NULL)
	{
		res = calculatetotext(exp);
	}
	else
	{
		res = (wchar_t*)malloc(14 * sizeof(wchar_t));
		memcpy(res, L"Syntax error!", 14 * sizeof(wchar_t));
	}

	if (res != NULL)
	{
		int size = wcslen(res);

		wchar_t *res2 = (wchar_t *)CoTaskMemAlloc((size + 1) * sizeof(wchar_t));
		res2 = (wchar_t *)memcpy(res2, res, (size + 1) * sizeof(wchar_t));

		free(res);

		return res2;
	}

	return NULL;
}