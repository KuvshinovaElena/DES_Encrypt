#pragma once
#include<bitset>
#include<fstream>
#include"PrepDES.h"

using std::bitset;
extern int n;

class DES
{
public:
	static std::ofstream& encrypt(std::ifstream&, bitset<56>, std::ofstream&);
	static std::ofstream& decrypt(std::ifstream&, bitset<56>, std::ofstream&);
protected:
	bitset<64> encryptBlock(bitset<64>);
	bitset<64> decryptBlock(bitset<64>);

};