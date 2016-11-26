#pragma once
#include<bitset>
#include<algorithm>
#include<string>
#include "Tables.h"

using std::bitset;
using namespace Tables;

namespace PrepDES {	
	template<int a, int b> bitset<b> permutation(const bitset<a>&, int *);	//Перестановки по таблицам
	template<int a> bitset<a> CyclShiftLeft(const bitset<a>&, int);		//Циклический сдвиг влево на n позиций
	void FeistelPermut(bitset<32>&, bitset<32>&, const bitset<48>*, bool mode);			//Перестановка Фейстеля
	bitset<32> funcFeistel(const bitset<32>&, const bitset<48>&);	//Функция Фейстеля
	bitset<32> newBlockFeistel(const bitset<48>&);					//Получение блоков B1, B2 ... B8
	bitset<48>* createSubKey(const bitset<56>&);			//Вычисление подключей ki
	bitset<64> addParityBits(const bitset<56>&);		//Добавление битов чётности в исходный ключ
	bitset <56> converterKey(const char*);				//Преобразование исходного ключа в битовый вектор (если необходимо)
	template<int a> bitset<a> reverse(const bitset<a>& bset);		//Реверс витового вектора
}

template<int a, int b>
bitset<b> PrepDES::permutation(const bitset<a>& block, int * table)
{
	bitset<b> newbl;
	for (size_t i = 0; i < b; i++)
	{
		newbl[i] = block[*(table + i) - 1];
	}
	return newbl;
}

template<int a>
bitset<a> PrepDES::CyclShiftLeft(const bitset<a>& block, int n)
{
	bitset<a> nblock = block;
	while (n) {
		bool d = nblock[0];
		nblock = nblock>>1;
		nblock[a-1] = nblock[a / 2 - 1];
		nblock[a / 2 - 1] = d;
		n--;
	}
	return nblock;
}

bitset<32> PrepDES::funcFeistel(const bitset<32>& block, const bitset<48>& subkey) {
	bitset<48> expbl = permutation<32, 48>(block, E);
	expbl ^= subkey;
	bitset<32> newblock = newBlockFeistel(expbl);
	newblock = permutation<32, 32>(newblock, P);
	return newblock;
}

bitset<32> PrepDES::newBlockFeistel(const bitset<48>& block)
{
	int byte = 0;
	bitset<32> newbl;
	while (byte < 8)
	{
		bitset<2> bl1;
		bl1[0] = block[byte * 6]; bl1[1] = block[6 * byte + 5];
		bl1 = reverse(bl1);
		bitset<4> bl2;
		bl2[0] = block[byte * 6 + 1]; bl2[1] = block[byte * 6 + 2]; bl2[2] = block[byte * 6 + 3]; bl2[3] = block[byte * 6 + 4];
		bl2 = reverse(bl2);
		int a = static_cast<int>(bl1.to_ulong());
		int b = static_cast<int>(bl2.to_ulong());
		bitset<4> newbyte(S[byte][a][b]);
		newbyte = reverse(newbyte);
		for (int i = 0; i < 4; i++)
		{
			newbl[byte * 4 + i] = newbyte[i];
		}
		byte++;
	}
	return newbl;
}

void PrepDES::FeistelPermut(bitset<32>& L, bitset<32>& R, const bitset<48>* subKey, bool mode)
{
	size_t n = 0;
	bitset<32> newL;
	bitset<32> newR;
	while (n < 16) {
		switch (mode)
		{
		case 0:
			newL = R;
			newR = L^funcFeistel(R, *(subKey + n));
			break;
		case 1:
			newL = R^funcFeistel(L, *(subKey + 15 - n));
			newR = L;
			break;
		}	
		n++;
		L = newL;
		R = newR;
	}
}

bitset<48>* PrepDES::createSubKey(const bitset<56>& key)
{
	bitset<48>* subKey = new bitset<48>[16];
	bitset<64> expKey = addParityBits(key);
	bitset<56> * nwKey = new bitset<56>[16];
	bitset<48> b;
	*nwKey = permutation<64, 56>(expKey, G);
	size_t n = 0;
	*nwKey = CyclShiftLeft(*nwKey, *(shift+n));
	while (n < 15)
	{
		*(nwKey + n + 1)  = CyclShiftLeft(*(nwKey + n), shift[n + 1]);
		n++;
	}
	for (int i = 0; i < 16; i++)
	{
		*(subKey+i) = permutation<56, 48>(*(nwKey+i), H);
	}
	for (int i = 0; i < 16; i++)
	{
		b = *(subKey + i);
	}
	delete[] nwKey;
	return subKey;
}

bitset<64> PrepDES::addParityBits(const bitset<56>& key)
{
	bitset<64> expKey;
	bool parity = 1;
	int i = 0;
	int j = 0;
	while (i < 56)
	{
		for (int k =0 ; k<7; k++,i++)
		{
			expKey[j++] = key[i];
			parity ^= key[i];
		}
		expKey[j++] = parity;
		parity = 1;
	}
	return expKey;
}

bitset <56> PrepDES::converterKey(const char* key)
{
	bitset<56> bitKey;
	int i = 0;
	for (;i<7;i++)
	{
		bitset<8> bitChar(*(key + i));
		for (int j = 0; j < 8; j++)
		{
			bitKey[i * 8 + j] = bitChar[j];
		}
	}
	return bitKey;
}

template<int a>
 bitset<a> PrepDES::reverse(const bitset<a>& bset)
{
	 std::string temp = bset.to_string();
	std::reverse(temp.begin(), temp.end());
	 bitset<a> nbset(temp);
	return nbset;
}