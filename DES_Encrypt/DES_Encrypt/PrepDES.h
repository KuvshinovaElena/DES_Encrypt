#pragma once
#include<bitset>
#include "Tables.h"

using std::bitset;

namespace PrepDES {
	using namespace Tables;
	template<typename B, int nBlckSz> B PrepDES::permutation(B, int[]);		//������������ �� ��������
	bitset<28> CyclShiftLeft(bitset<28>, int);		//����������� ����� ����� �� n �������
	//bitset<64> permut(bitset<64>, int *);			//��������� � �������� ������������ (int *: IP ��� FP)
	//bitset<48> expansion(bitset<32>);				//������� ���������� �
	bitset<64> FeistelPermut(bitset<64>);			//������������ ��������
	bitset<32> funcFeistel(bitset<32>, bitset<48>);	//������� ��������
	bitset<48> createSubKey(bitset<56>);			//���������� ���������
	bitset<64> permutG(bitset<64>);					//��������� �������������� �����
	bitset<48> permutH(bitset<64>);					//��������� �������������� �����
}


bitset<28> PrepDES::CyclShiftLeft(bitset<28> block, int n)
{
	while (n) {
		static bool d = block[0];
		block <<= 1;
		block[27] = d;
		block = CyclShiftLeft(block, --n);
	}
	return block;
}

template<typename B, int nBlckSz> B PrepDES::permutation(B block, int table[])
{
	B<nBlckSz> newbl;
	for (size_t i = 0; i < nBlckSz; i++)
	{
		newbl[i] = block[*(table + i) - 1];
	}
	return newbl;
}