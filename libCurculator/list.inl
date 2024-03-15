#pragma once

#include "list.h"

namespace curculator
{
	template<typename T>
	List<T>::List() : List(0) {}

	template<typename T>
	List<T>::List(int len)
	{
		SetLength(len);
	}

	template<typename T>
	void List<T>::Add(T el)
	{
		SetLength(len + 1);
		Set(len - 1, el);
	}

	template<typename T>
	T List<T>::Get(int id)
	{
		int i = id;

		Block *blk = &main;

		while (i > BLOCKSIZE)
		{
			blk = blk->next;
			i -= BLOCKSIZE;
		}

		return blk->el[i];
	}

	template<typename T>
	void List<T>::Set(int id, T el)
	{
		int i = id;

		Block *blk = &main;

		while (i > BLOCKSIZE)
		{
			blk = blk->next;
			i -= BLOCKSIZE;
		}

		blk->el[i] = el;
	}

	template<typename T>
	int List<T>::GetLength()
	{
		return len;
	}

	template<typename T>
	void List<T>::SetLength(int len)
	{
		int newcount = (len / BLOCKSIZE) + 1;

		Block *blk = main.next;

		for (int i = 1; i < newcount; i++, blk = blk->next)
		{
			if (blk->next == NULL)
			{
				blk->next = new Block();
				blk->next->next = NULL;
			}

			if (i + 1 == newcount)
			{
				blk->next = NULL;
				blk = blk->next;

				for (int j = i + 1; j < newcount; j++)
				{
					if (blk != NULL)
					{
						Block *next = blk->next;
						delete blk;
						blk = next;
					}
					else break;
				}
			}
		}

		this->len = len;
	}
}