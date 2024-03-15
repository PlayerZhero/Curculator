#include "pch.h"
#include "lexic.h"

#include <math.h>

#include <stdio.h>

namespace curculator
{
	namespace lexic
	{
		Token::Token(eToken tokenType)
		{
			this->tokenType = tokenType;
		}

		Literal::Literal(FixedReal val) : Token(TOKEN_LITERAL)
		{
			this->val = val;
		}

		System::System(unsigned long long sys) : Token(TOKEN_SYSTEM)
		{
			this->sys = sys;
		}
	}

	Lexer::Lexer(const wchar_t *input)
	{
		this->input = input;
		pos = 0;
		len = wcslen(input);

		tokens = new List<lexic::Token*>();

		lexic::Token *tok;

		while ((tok = Scan()) != NULL)
		{
			tokens->Add(tok);
		}
	}

	wchar_t Lexer::Get(int pos)
	{
		if (pos < len)
		{
			wchar_t c = input[pos];
			return c;
		}

		return L'\0';
	}

	wchar_t Lexer::Peek()
	{
		pos++;
		return Get(pos - 1);
	}

	lexic::Token *Lexer::Scan()
	{
		wchar_t c1 = Peek();

		while (c1 == L' ' || c1 == L'\n')
		{
			c1 = Peek();
		}

		if (c1 == L'\0') return NULL;

		if (c1 == L'*' && Get(pos) == L'*')
		{
			pos++;
			return new lexic::Token(lexic::TOKEN_POW);
		}

		switch (c1)
		{
		case L'+': return new lexic::Token(lexic::TOKEN_ADD);
		case L'-': return new lexic::Token(lexic::TOKEN_SUB);
		case L'*': return new lexic::Token(lexic::TOKEN_MUL);
		case L'/': return new lexic::Token(lexic::TOKEN_DIV);
		case L'>': return new lexic::Token(lexic::TOKEN_CONV);
		case L'(': return new lexic::Token(lexic::TOKEN_OPH);
		case L')': return new lexic::Token(lexic::TOKEN_CPH);
		}

		if (c1 >= L'\x2080' && c1 <= L'\x2089')
		{
			int posx = pos - 1;

			int sys = 0;

			wchar_t c2;

			while ((c2 = Get(posx)) != '\0' && c2 >= L'\x2080' && c2 <= L'\x2089')
			{
				sys *= 10;
				sys += (int)(c2 - L'\x2080');
				posx++;
			}

			pos = posx;

			return new lexic::System(sys);
		}

		static const wchar_t *numCharSet = L"VUTSRQPONMLKJIHGFEDCBA9876543210";

		if (wcschr(numCharSet, c1) != NULL)
		{
			int posx = pos;

			while (Get(posx) != L'\0' && (wcschr(numCharSet, Get(posx)) != NULL || Get(posx) == L'.'))
			{
				posx++;
			}

			int sys = 0;

			if (Get(posx) >= L'\x2080' && Get(posx) <= L'\x2089')
			{
				wchar_t c2;

				while ((c2 = Get(posx)) != '\0' && c2 >= L'\x2080' && c2 <= L'\x2089')
				{
					sys *= 10;
					sys += (int)(c2 - L'\x2080');
					posx++;
				}
			}
			else sys = 10;

			int nlen = 0;
			wchar_t num[128];

			wchar_t c2;

			pos--;

			while ((c2 = Peek()) != '\0' && wcschr(&numCharSet[32 - sys], c2) != NULL || c2 == L'.')
			{
				num[nlen] = c2;
				nlen++;
			}

			num[nlen] = L'\0';

			pos = posx;

			FixedReal ipart = FixedReal(false, 0, 0);
			FixedReal fpart = FixedReal(false, 0, 0);

			int dotpos = wcschr(num, '.') != NULL ? (int)(wcschr(num, '.') - (wchar_t*)&num) : -1;

			int ipartend = 0;
			if (dotpos != -1)
			{
				ipartend = dotpos;
			}
			else ipartend = nlen;

			//for (int i = 0; i < ipartend; i++)
			//{
			//	int n = 31 - (int)(wcschr(numCharSet, num[i]) - numCharSet);

			//	ipart = FixedReal::Add(ipart, FixedReal(false, n * pow(sys, ipartend - i - 1), 0));
			//}

			for (int i = 0; i < ipartend; i++)
			{
				int n = 31 - (int)(wcschr(numCharSet, num[i]) - numCharSet);

				ipart = FixedReal::Mul(ipart, FixedReal(false, sys, 0));
				ipart = FixedReal::Add(ipart, FixedReal(false, n, 0));
			}

			if (dotpos != -1)
			{
				//for (int i = dotpos + 1; i < nlen; i++)
				//{
				//	int n = 31 - (int)(wcschr(numCharSet, num[i]) - numCharSet);

				//	FixedReal fpp = FixedReal::Div(FixedReal(false, n, 0), FixedReal(false, pow(sys, (i - dotpos)), 0));
				//	printf("fpp %ws\n", fpp.Print(2));
				//	fpart = FixedReal::Add(fpart, fpp);
				//}

				for (int i = nlen - 1; i >= dotpos + 1; i--)
				{
					int n = 31 - (int)(wcschr(numCharSet, num[i]) - numCharSet);

					fpart = FixedReal::Add(fpart, FixedReal(false, n, 0));
					fpart = FixedReal::Div(fpart, FixedReal(false, sys, 0));
				}
			}

			return new lexic::Literal(FixedReal::Add(ipart, fpart));
		}

		return NULL;
	}

	List<lexic::Token*> *Lexer::GetTokens()
	{
		return tokens;
	}
}