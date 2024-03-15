#pragma once

#define BLOCKSIZE 256

namespace curculator
{
	template<typename T>
	class List
	{
	private:
		struct Block
		{
		public:
			T el[BLOCKSIZE];
			Block *next;
		};

		Block main;
		int len;

	public:
		List();

		List(int len);

		void Add(T el);

		T Get(int id);

		void Set(int id, T el);

		int GetLength();

		void SetLength(int len);
	};
}

#include "list.inl"