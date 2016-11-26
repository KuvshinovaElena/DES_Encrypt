#pragma once
#include<bitset>
#include<fstream>
#include<iostream>
#include"PrepDES.h"

using std::bitset;
using std::ofstream;
using std::ifstream;
using namespace PrepDES;

namespace DES
{
	bool encrypt(const char *, const bitset<56>&, bool mode, const char* = "output.txt");	//Шифрование DES
	bitset<64> encryptBlock(bitset<64>&, const bitset<56>&);	//Шифрование блока
	bitset<64> decryptBlock(bitset<64>&, const bitset<56>&);	//Расшифрование блока
};

bool DES::encrypt(const char* ifile, const bitset<56>& key, bool mode, const char* ofile)
{
	ifstream in(ifile, std::ios::binary|std::ios::in);
	ofstream out(ofile, std::ios::binary|std::ios::out);
	if (!in.is_open() || !out.is_open())
	{
		return 0;
	}
	bitset<64> block;
	int count = 0;
	int pos = static_cast<int>(in.tellg());
	while (pos != EOF)
	{
		in.read(reinterpret_cast<char*>(&block), 8);
		count++;
		pos = static_cast<int>(in.tellg());
		if (pos<0)
		{
			in.clear();
			pos = static_cast<int>(in.tellg());
			pos = pos * 8 - (count - 1) * 64 - 1;
			if (pos < 0) break;
			for (int i = pos; i < 64 && i>0; i++)
			{
				block[i] = 0;
			}
			pos = EOF;	
		}
		switch (mode)
		{
		case 0:
			block = encryptBlock(block, key);
			break;
		case 1:
			block = decryptBlock(block, key);
			break;
		}
		out.write(reinterpret_cast<char*>(&block),8);
	}
	in.close();
	out.close();
	return 1;
}

bitset<64> DES::encryptBlock(bitset<64>& block, const bitset<56>& key)
{
	block = permutation<64, 64>(block, IP);
	bitset<48> * subKey = createSubKey(key);
	bitset<32> L, R;
	for (int i = 0; i<32;i++)
	{
		L[i] = block[i];
		R[i] = block[i + 32];
	}
	FeistelPermut(L,R,subKey,0);
	for (int i = 0; i < 32; i++)
	{
		block[i] = L[i];
		block[i + 32] = R[i];
	}
	block = permutation<64, 64>(block,FP);
	delete[] subKey;
	return block;
}

bitset<64> DES::decryptBlock(bitset<64>& block, const bitset<56>& key)
{
	block = permutation<64, 64>(block, IP);
	bitset<48> * subKey = createSubKey(key);
	bitset<32> L, R;
	for (int i = 0; i<32; i++)
	{
		L[i] = block[i];
		R[i] = block[i + 32];
	}
	FeistelPermut(L, R, subKey,1);
	for (int i = 0; i < 32; i++)
	{
		block[i] = L[i];
		block[i + 32] = R[i];
	}
	block = permutation<64, 64>(block, FP);
	return block;
}