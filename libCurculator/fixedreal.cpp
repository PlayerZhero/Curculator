#include "pch.h"
#include "fixedreal.h"

#include <stdio.h>
#include <math.h>
#include <malloc.h>

namespace curculator
{
	FixedReal::FixedReal() {}

	FixedReal::FixedReal(bool negative, unsigned long long ipart,
#if FSIZE == 32
		unsigned int fpart
#elif FSIZE == 64
		unsigned long long fpart
#endif
	)
	{
		this->negative = negative;
		this->ipart = ipart;
		this->fpart = fpart;
	}

	bool FixedReal::IsNegative()
	{
		return negative;
	}

	void FixedReal::SetNegative(bool neg)
	{
		negative = neg;
	}

	unsigned char FixedReal::GetIPartBit(unsigned char bit)
	{
		if (bit < 0 || bit >= ISIZE) return 0;

		unsigned char res = ((ipart & ((unsigned long long)1 << bit)) >> bit) != 0;

		return res;
	}

	unsigned char FixedReal::GetFPartBit(unsigned char bit)
	{
		if (bit < 0 || bit >= FSIZE) return 0;

		unsigned char res = ((fpart & ((unsigned long long)1 << bit)) >> bit) != 0;

		return res;
	}

	void FixedReal::SetIPartBit(unsigned char bit, unsigned char val)
	{
		if (bit < 0 || bit >= ISIZE) return;

		ipart = (ipart & ~((unsigned long long)1 << bit)) | (unsigned long long)val << bit;
	}

	void FixedReal::SetFPartBit(unsigned char bit, unsigned char val)
	{
		if (bit < 0 || bit >= FSIZE) return; // when there was no this line calculations was broken

		fpart = (fpart & ~((unsigned long long)1 << bit)) | (unsigned long long)val << bit;
	}

	unsigned char FixedReal::GetBit(int bit)
	{
		if (bit < 0)
		{
			return GetFPartBit((unsigned char)(FSIZE + bit));
		}
		else return GetIPartBit((unsigned char)bit);
	}

	void FixedReal::SetBit(int bit, unsigned char val)
	{
		if (bit < 0)
		{
			SetFPartBit((unsigned char)(FSIZE + bit), val);
		}
		else SetIPartBit((unsigned char)bit, val);
	}

	wchar_t *FixedReal::Print(int sys)
	{
		if (sys == 2)
		{
			int isize = 0, fsize = 0;

			for (int i = ISIZE - 1; i >= 0; i--)
			{
				if (GetIPartBit(i) != 0)
				{
					isize = i + 1;
					break;
				}

				if (i == 0) isize = 1;
			}

			for (int i = 0; i < FSIZE; i++)
			{
				if (GetFPartBit(i) != 0)
				{
					fsize = FSIZE - i;
					break;
				}
			}

			int size = isize + fsize + (fsize > 0 ? 1 : 0) + (negative ? 1 : 0);

			wchar_t *chrs = (wchar_t*)malloc((size + 1) * sizeof(wchar_t));

			int pos = 0;
			if (negative)
			{
				chrs[pos] = L'-';
				pos++;
			}

			for (int i = isize - 1; i >= 0; i--, pos++)
			{
				chrs[pos] = GetIPartBit(i) + '0';
			}
			if (fsize > 0)
			{
				chrs[pos] = '.';
				pos++;
				for (int i = fsize - 1; i >= 0; i--, pos++)
				{
					chrs[pos] = GetFPartBit(i + FSIZE - fsize) + '0';
				}
			}
			chrs[size] = '\0';

			return chrs;
		}
		else
		{
			const wchar_t *charSet = L"VUTSRQPONMLKJIHGFEDCBA9876543210";

			FixedReal temp = *this;

			wchar_t ip[128], fp[128];
			int iplen = 0, fplen = 0;

			if (temp.ipart > 0)
			{
				for (FixedReal ipt = temp; ipt.ipart > 0;)
				{
					ipt.fpart = 0;
					ipt = Div(ipt, FixedReal(false, sys, 0));
					FixedReal iptt = ipt;
					iptt.ipart = 0;
					iptt = Mul(iptt, FixedReal(false, sys, 0));
					iptt = Round(iptt);
					ip[iplen] = charSet[31 - iptt.ipart];
					iplen++;
				}
			}
			else
			{
				ip[0] = '0';
				iplen++;
			}

			for (FixedReal fpt = temp; fpt.fpart > 0;)
			{
				fpt.ipart = 0;
				fpt = Mul(fpt, FixedReal(false, sys, 0));
				fp[fplen] = charSet[31 - fpt.ipart];
				fplen++;
			}

			int len = iplen + (negative ? 1 : 0);

			if (fplen > 0)
			{
				len += fplen + 1;
			}

			wchar_t *str = new wchar_t[len + 1];

			if (negative)
			{
				str[0] = L'-';
			}

			for (int i = 0; i < iplen; i++)
			{
				str[iplen - i - 1 + (negative ? 1 : 0)] = ip[i];
			}

			if (fplen > 0)
			{
				str[iplen + (negative ? 1 : 0)] = '.';

				for (int i = 0; i < fplen; i++)
				{
					str[i + iplen + 1 + (negative ? 1 : 0)] = fp[i];
				}
			}

			str[len] = '\0';

			return str;
		}
	}

	FixedReal FixedReal::LShift(FixedReal a, unsigned char bits)
	{
		FixedReal res = a;

		res.ipart <<= bits;

		for (int i = 0; i < bits; i++)
		{
			res.SetIPartBit(i, res.GetFPartBit(FSIZE + i - bits));
		}

		res.fpart <<= bits;

		return res;
	}

	FixedReal FixedReal::RShift(FixedReal a, unsigned char bits)
	{
		FixedReal res = a;

		res.fpart >>= bits;

		for (int i = 0; i < bits; i++)
		{
			res.SetFPartBit(FSIZE + i - bits, res.GetIPartBit(i));
		}

		res.ipart >>= bits;

		return res;
	}

	FixedReal FixedReal::Shift(FixedReal a, int bits)
	{
		if (bits > 0)
		{
			return LShift(a, (unsigned char)bits);
		}
		else if (bits < 0)
		{
			return RShift(a, (unsigned char)-bits);
		}

		return a;
	}

	bool FixedReal::IsBelow(FixedReal a, FixedReal b)
	{
		if (!a.negative && !b.negative)
		{
#if FSIZE == 32
			unsigned int aih = ((unsigned int*)&a.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
			unsigned int ail = ((unsigned int*)&a.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
			unsigned int af = a.fpart, bf = b.fpart;

			__asm
			{
				mov eax, aih
				cmp eax, bih
				ja above
				jb below
				mov eax, ail
				cmp eax, bil
				ja above
				jb below
				mov eax, af
				cmp eax, bf
				ja above
				jb below
			}

		above:
			return false;

		below:
			return true;
#elif FSIZE == 64
			unsigned int aih = ((unsigned int*)&a.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
			unsigned int ail = ((unsigned int*)&a.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
			unsigned int afh = ((unsigned int*)&a.fpart)[1], bfh = ((unsigned int*)&b.fpart)[1];
			unsigned int afl = ((unsigned int*)&a.fpart)[0], bfl = ((unsigned int*)&b.fpart)[0];

			__asm
			{
				mov eax, aih
				cmp eax, bih
				ja above
				jb below
				mov eax, ail
				cmp eax, bil
				ja above
				jb below
				mov eax, afh
				cmp eax, bfh
				ja above
				jb below
				mov eax, afl
				cmp eax, bfl
				ja above
				jb below
			}

		above:
			return false;

		below:
			return true;
#endif
		}
		else if (a.negative && b.negative)
		{
			a.negative = false;
			b.negative = false;

			return IsBelow(b, a);
		}
		else if (!a.negative && b.negative)
		{
			return false;
		}
		else if (a.negative && !b.negative)
		{
			return true;
		}
	}

	bool FixedReal::IsAbove(FixedReal a, FixedReal b)
	{
		return IsBelow(b, a);
	}

	bool FixedReal::IsEquals(FixedReal a, FixedReal b)
	{
		return !IsBelow(a, b) && !IsAbove(a, b);
	}

	int FixedReal::BitScanReverse(FixedReal a)
	{
		int leftbit = -1;

		for (int i = ISIZE - 1; i >= 0; i--)
		{
			if (a.GetIPartBit(i) == 1)
			{
				leftbit = i;
				break;
			}
		}

		if (leftbit == -1)
		{
			for (int i = FSIZE - 1; i >= 0; i--)
			{
				if (a.GetFPartBit(i) == 1)
				{
					leftbit = -(FSIZE - i);
					break;
				}
			}
		}

		return leftbit;
	}

	FixedReal FixedReal::Add(FixedReal a, FixedReal b)
	{
		if (!a.negative && !b.negative)
		{
#if FSIZE == 32
			FixedReal res = a;

			unsigned int rih = ((unsigned int*)&res.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
			unsigned int ril = ((unsigned int*)&res.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
			unsigned int rf = res.fpart, bf = b.fpart;

			__asm
			{
				mov eax, rf
				mov ecx, ril
				mov edx, rih
				add eax, bf
				adc ecx, bil
				adc edx, bih
				mov rf, eax
				mov ril, ecx
				mov rih, edx
			}

			((unsigned int*)&res.ipart)[1] = rih;
			((unsigned int*)&res.ipart)[0] = ril;
			res.fpart = rf;

			res.negative = false;

			return res;
#elif FSIZE == 64
			FixedReal res = a;

			unsigned int rih = ((unsigned int*)&res.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
			unsigned int ril = ((unsigned int*)&res.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
			unsigned int rfh = ((unsigned int*)&res.fpart)[1], bfh = ((unsigned int*)&b.fpart)[1];
			unsigned int rfl = ((unsigned int*)&res.fpart)[0], bfl = ((unsigned int*)&b.fpart)[0];

			__asm
			{
				mov eax, rfl
				mov ebx, rfh
				mov ecx, ril
				mov edx, rih
				add eax, bfl
				adc ebx, bfh
				adc ecx, bil
				adc edx, bih
				mov rfl, eax
				mov rfh, ebx
				mov ril, ecx
				mov rih, edx
			}

			((unsigned int*)&res.ipart)[1] = rih;
			((unsigned int*)&res.ipart)[0] = ril;
			((unsigned int*)&res.fpart)[1] = rfh;
			((unsigned int*)&res.fpart)[0] = rfl;

			res.negative = false;

			return res;
#endif
		}
		else if (a.negative && b.negative)
		{
			a.negative = false;
			b.negative = false;

			FixedReal res = Add(a, b);
			res.negative = true;

			return res;
		}
		else if (!a.negative && b.negative)
		{
			a.negative = false;
			b.negative = false;

			FixedReal res = Sub(a, b);

			return res;
		}
		else if (a.negative && !b.negative)
		{
			a.negative = false;
			b.negative = false;

			FixedReal res = Sub(b, a);

			return res;
		}
	}

	FixedReal FixedReal::Sub(FixedReal a, FixedReal b)
	{
		if (!a.negative && !b.negative)
		{
			if (IsEquals(a, b))
			{
				return FixedReal(false, 0, 0);
			}
			else if (IsAbove(a, b))
			{
#if FSIZE == 32
				FixedReal res = a;

				unsigned int rih = ((unsigned int*)&res.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
				unsigned int ril = ((unsigned int*)&res.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
				unsigned int rf = res.fpart, bf = b.fpart;

				__asm
				{
					mov eax, rf
					mov ecx, ril
					mov edx, rih
					sub eax, bf
					sbb ecx, bil
					sbb edx, bih
					mov rf, eax
					mov ril, ecx
					mov rih, edx
				}

				((unsigned int*)&res.ipart)[1] = rih;
				((unsigned int*)&res.ipart)[0] = ril;
				res.fpart = rf;

				return res;
#elif FSIZE == 64
				FixedReal res = a;

				unsigned int rih = ((unsigned int*)&res.ipart)[1], bih = ((unsigned int*)&b.ipart)[1];
				unsigned int ril = ((unsigned int*)&res.ipart)[0], bil = ((unsigned int*)&b.ipart)[0];
				unsigned int rfh = ((unsigned int*)&res.fpart)[1], bfh = ((unsigned int*)&b.fpart)[1];
				unsigned int rfl = ((unsigned int*)&res.fpart)[0], bfl = ((unsigned int*)&b.fpart)[0];

				__asm
				{
					mov eax, rfl
					mov ebx, rfh
					mov ecx, ril
					mov edx, rih
					sub eax, bfl
					sbb ebx, bfh
					sbb ecx, bil
					sbb edx, bih
					mov rfl, eax
					mov rfh, ebx
					mov ril, ecx
					mov rih, edx
				}

				((unsigned int*)&res.ipart)[1] = rih;
				((unsigned int*)&res.ipart)[0] = ril;
				((unsigned int*)&res.fpart)[1] = rfh;
				((unsigned int*)&res.fpart)[0] = rfl;

				return res;
#endif
			}
			else
			{
				FixedReal res = Sub(b, a);

				res.negative = true;

				return res;
			}
		}
		else if (a.negative && b.negative)
		{
			a.negative = false;
			b.negative = false;

			FixedReal res = Sub(b, a);

			return res;
		}
		else if (!a.negative && b.negative)
		{
			b.negative = false;

			FixedReal res = Add(a, b);

			return res;
		}
		else if (a.negative && !b.negative)
		{
			a.negative = false;

			FixedReal res = Add(a, b);
			res.negative = true;

			return res;
		}
	}

	FixedReal FixedReal::Mul(FixedReal a, FixedReal b)
	{
		FixedReal sum = FixedReal(false, 0, 0);

		bool negative = a.negative != b.negative;

		a.negative = false;
		b.negative = false;

		for (int i = 0; i < ISIZE; i++)
		{
			if (a.GetIPartBit(i) == 1)
			{
				sum = FixedReal::Add(sum, FixedReal::LShift(b, i));
			}
		}
		for (int i = 0; i < FSIZE; i++)
		{
			if (a.GetFPartBit(i) == 1)
			{
				sum = FixedReal::Add(sum, FixedReal::RShift(b, FSIZE - i));
			}
		}

		sum.negative = negative;

		return sum;
	}

	// https://studfile.net/preview/9699761/page:2/
	FixedReal FixedReal::Div(FixedReal a, FixedReal b)
	{
		FixedReal q = FixedReal(false, 0, 0);

		bool negative = a.negative != b.negative;

		a.negative = false;
		b.negative = false;

		FixedReal tempa = a;

		int aleft = BitScanReverse(a), bleft = BitScanReverse(b);

		for (int i = aleft; i >= -FSIZE; i--) // long division algorithm
		{
			//printf("%ws - %ws = ", tempa.Print(2), Shift(b, i - bleft).Print(2));
			tempa = Sub(tempa, Shift(b, i - bleft));
			//printf("%ws with %wc\n", tempa.Print(2), tempa.negative ? '-' : '+');

			if (!tempa.negative)
			{
				q.SetBit(i - bleft, 1);
			}
			else
			{
				q.SetBit(i - bleft, 0);
				tempa = Add(tempa, Shift(b, i - bleft));
			}

			if (IsEquals(tempa, FixedReal(false, 0, 0)))
			{
				break;
			}
		}

		q.negative = negative;

		return q;
	}

	FixedReal FixedReal::Round(FixedReal a)
	{
		if (
#if FSIZE == 32
			a.fpart >= (1 << 31)
#elif FSIZE == 64
			a.fpart >= ((unsigned long long)1 << 63)
#endif
			) // if the fpart >= 0.5
		{
			a.ipart++;
			a.fpart = 0;
			return a;
		}
		else
		{
			a.fpart = 0;
			return a;
		}
	}

	FixedReal FixedReal::Abs(FixedReal a)
	{
		if (a.negative) a.negative = false;

		return a;
	}

	FixedReal FixedReal::Neg(FixedReal a)
	{
		a.negative = !a.negative;

		return a;
	}

	FixedReal FixedReal::Pow(FixedReal a, FixedReal b)
	{
		bool negative = b.negative;
		b.negative = false;

		FixedReal result = FixedReal(false, 0, 0);

		if (b.fpart == 0) // if the number is integer
		{
			if (b.ipart == 0)
			{
				result = FixedReal(false, 1, 0);
			}
			else if (b.ipart == 1)
			{
				result = a;
			}
			else if (b.ipart == 2)
			{
				result = Mul(a, a);
			}
			else if (b.ipart % 2 == 0)
			{
				FixedReal tempb = b;
				tempb.ipart /= 2;

				result = Pow(Pow(a, tempb), FixedReal(false, 2, 0));
			}
			else
			{
				FixedReal tempb = b;
				tempb.ipart -= 1;
				tempb.ipart /= 2;

				result = Mul(Pow(Pow(a, tempb), FixedReal(false, 2, 0)), a);
			}
		}
		else
		{
			unsigned long long m, n = 1;

			FixedReal tempb = b;

			while (tempb.fpart != 0) // converting real to the usual fraction
			{
				tempb = LShift(tempb, 1);
				n *= 2;

				if ((n & ((unsigned long long)1 << (FSIZE - 1)))) // check the overflow
				{
					break;
				}
			}
			m = tempb.ipart;

			unsigned long long gcd = 0;

			unsigned long long tempm = m, tempn = n;

			while (true) // finding gcd
			{
				tempm = min(tempm, tempn);
				tempn = max(tempm, tempn) - tempm;

				if (tempn == 0)
				{
					gcd = tempm;
					break;
				}
			}

			m = round((double)m / gcd);
			n = round((double)n / gcd);

			result = Pow(a, FixedReal(false, m, 0));

			if (n != 0)
			{
				//FixedReal rn = a;
				//FixedReal root = Div(a, FixedReal(false, n, 0));
				//FixedReal rootprev = FixedReal(false, 0, 0);

				//while (!IsBelow(Abs(Sub(rootprev, root)), FixedReal(false, 0, (unsigned long long)1 << 60)))
				//{
				//	//printf("%ws\n", root.Print(10));
				//	FixedReal rnn = Pow(
				//		root,
				//		FixedReal(false, n - 1, 0)
				//	);
				//	rn = Div(
				//		a,
				//		rnn
				//	);
				//	rootprev = root;
				//	root = Div(
				//				Add(rn, root),
				//				FixedReal(false, 2, 0)
				//			);
				//}

				//result = Mul(Add(root, rootprev), FixedReal(false, 0, (unsigned long long)1 << (FSIZE - 1)));

				//FixedReal root = Div(result, FixedReal(false, 2, 0));
				//FixedReal f;

				//while (!IsEquals(f, FixedReal(false, 0, 0)))
				//{
				//	printf("%ws\n", root.Print(10));

				//	f = Sub(Pow(root, FixedReal(false, n, 0)), result);

				//	FixedReal dx = Div(Neg(f), Mul(FixedReal(false, n, 0), root));
				//	root = Abs(Add(root, dx));
				//}

				// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%BD%D0%B0%D1%85%D0%BE%D0%B6%D0%B4%D0%B5%D0%BD%D0%B8%D1%8F_%D0%BA%D0%BE%D1%80%D0%BD%D1%8F_n-%D0%BD%D0%BE%D0%B9_%D1%81%D1%82%D0%B5%D0%BF%D0%B5%D0%BD%D0%B8
				// https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%9D%D1%8C%D1%8E%D1%82%D0%BE%D0%BD%D0%B0

				FixedReal root = Div(result, FixedReal(false, 2, 0));
				FixedReal rootprev = FixedReal(false, 0, 0);

				while (!IsBelow(Abs(Sub(root, rootprev)), FixedReal(false, 0, (unsigned long long)1 << 32)))
				{
					rootprev = root;
					root = Sub(root, Div(Sub(Pow(root, FixedReal(false, n, 0)), result), Mul(FixedReal(false, n, 0), Pow(root, FixedReal(false, n - 1, 0)))));

					//printf("%ws\n", root.Print(10));
				}

				result = root;
			}
		}

		if ((result.ipart != 0 || result.fpart != 0) && negative)
		{
			result = Div(FixedReal(false, 1, 0), result);
		}

		return result;
	}
}