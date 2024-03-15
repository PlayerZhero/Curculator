#pragma once

#define ISIZE 64
#define FSIZE 64

#undef BitScanReverse

namespace curculator
{
	struct FixedReal
	{
	private:
		bool negative;
		unsigned long long ipart;
#if FSIZE == 32
		unsigned int fpart;
#elif FSIZE == 64
		unsigned long long fpart;
#endif

	public:
		FixedReal();

		FixedReal(bool negative, unsigned long long ipart,
#if FSIZE == 32
			unsigned int fpart
#elif FSIZE == 64
			unsigned long long fpart
#endif
		);

		bool IsNegative();

		void SetNegative(bool neg);

		unsigned char GetIPartBit(unsigned char bit);

		unsigned char GetFPartBit(unsigned char bit);

		void SetIPartBit(unsigned char bit, unsigned char val);

		void SetFPartBit(unsigned char bit, unsigned char val);

		unsigned char GetBit(int bit);

		void SetBit(int bit, unsigned char val);

		wchar_t *Print(int sys);

		static FixedReal LShift(FixedReal a, unsigned char bits);

		static FixedReal RShift(FixedReal a, unsigned char bits);

		static FixedReal Shift(FixedReal a, int bits);

		static bool IsBelow(FixedReal a, FixedReal b);

		static bool IsAbove(FixedReal a, FixedReal b);

		static bool IsEquals(FixedReal a, FixedReal b);

		static int BitScanReverse(FixedReal a);

		static FixedReal Add(FixedReal a, FixedReal b);

		static FixedReal Sub(FixedReal a, FixedReal b);

		static FixedReal Mul(FixedReal a, FixedReal b);

		static FixedReal Div(FixedReal a, FixedReal b);
		
		static FixedReal Round(FixedReal a);
		
		static FixedReal Abs(FixedReal a);
		
		static FixedReal Neg(FixedReal a);

		static FixedReal Pow(FixedReal a, FixedReal b);
	};
}