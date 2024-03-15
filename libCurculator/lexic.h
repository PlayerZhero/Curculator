#pragma once

#include "fixedreal.h"
#include "list.h"

namespace curculator
{
	namespace lexic
	{
		enum eToken : unsigned char
		{
			TOKEN_NAT = 0,

			TOKEN_LITERAL = 5,
			TOKEN_SYSTEM,

			TOKEN_ADD = 10,
			TOKEN_SUB,
			TOKEN_MUL,
			TOKEN_DIV,
			TOKEN_POW,
			TOKEN_CONV,
			TOKEN_OPH,
			TOKEN_CPH
		};

		struct Token
		{
		public:
			eToken tokenType;

			Token(eToken tokenType);
		};

		struct Literal : public Token
		{
		public:
			FixedReal val;

			Literal(FixedReal val);
		};

		struct System : public Token
		{
		public:
			unsigned long long sys;

			System(unsigned long long sys);
		};
	}

	class Lexer
	{
	private:
		int pos;
		int len;
		const wchar_t *input;

		List<lexic::Token*> *tokens;

	public:
		Lexer(const wchar_t *input);

		wchar_t Get(int pos);

		wchar_t Peek();

		lexic::Token *Scan();

		List<lexic::Token*> *GetTokens();
	};
}