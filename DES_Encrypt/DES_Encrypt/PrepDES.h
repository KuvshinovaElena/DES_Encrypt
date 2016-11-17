#pragma once
#include<bitset>
#include<vector>
#include<list>
#include "Tables.h"

using std::bitset;

namespace PrepDES {
	using namespace Tables;
	template<int a, int b> bitset<b> permutation(const bitset<a>&, int *);	//Перестановки по таблицам
	template<int a> bitset<a> CyclShiftLeft(const bitset<a>&, int);		//Циклический сдвиг влево на n позиций
	bitset<64> FeistelPermut(const bitset<32>&, const bitset<32>&, std::vector<bitset<48>>);			//Перестановка Фейстеля
	bitset<32> funcFeistel(const bitset<32>&, const bitset<48>&);	//Функция Фейстеля
	bitset<32> newBlockFeistel(const bitset<48>&, int);
	bitset<48> createSubKey(const bitset<56>&);			//Вычисление подключей
}

template<int a, int b>
bitset<b> PrepDES::permutation(const bitset<a>& bclock, int * table)
{
	bitset<b>& newbl;
	for (size_t i = 0; i < b; i++)
	{
		newbl[i] = block[*(table + i) - 1];
	}
	return newbl;
}

template<int a>
bitset<a> PrepDES::CyclShiftLeft(const bitset<a>& block, int n)
{
	while (n) {
		static bool d = block[0];
		block <<= 1;
		block[N-1] = d;
		block = CyclShiftLeft(block, --n);
	}
	return block;
}

bitset<32> PrepDES::funcFeistel(const bitset<32>& block, const bitset<48>& subkey) {
	bitset<48> expbl = permutation<32, 48>(block, E);
	expbl = expbl^subkey;
	bitset<32> newblock = newBlockFeistel(expbl, 0);
	newblock = permutation<32, 32>(newblock, P);
	return newblock;
}

bitset<32> PrepDES::newBlockFeistel(const bitset<48>& block, int byte)
{
	bitset<2> bl1;
	bl1[0] = block[byte*8]; bl1[2] = block[8*(byte + 1)];
	bitset<4> bl2;
	bl2[0] = block[byte*8+1]; bl2[1] = block[byte * 8 + 2]; bl2[2] = block[byte * 8 + 3]; bl2[3] = block[byte * 8 + 4];
	int a = static_cast<int>(bl1.to_ulong()) - 1;
	int b = static_cast<int>(bl2.to_ulong()) - 1;
	bitset<4> newbyte(S[byte][a][b]);
	static bitset<32> newbl;
	for (int i = 0; i < 4; i++)
	{
		newbl[byte * 8 + i] = newbyte[i];
	}
	newbl = newBlockFeistel(block, ++byte);
	return newbl;
}

bitset<64> PrepDES::FeistelPermut(const bitset<32>& L, const bitset<32>& R, std::vector<bitset<48>> subKey)
{
	static size_t n = 0;
	bitset<32> newL;
	bitset<32> newR;
	while (n < 15) {
		newL = R;
		newR = L^funcFeistel(R,subKey[n]);
		n++;
		FeistelPermut(newL, newR, subKey);
	}
	bitset<64> newBl;
	for (int i = 0; i < 31; i++)
	{
		newBl[i] = newL[i];
		newBl[i + 31] = newR[i];
	}
	return permutation<64, 64>(newBl, P);
}

bitset<48> PrepDES::createSubKey(const bitset<56>& key)
{
	
}
