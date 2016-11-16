#pragma once
#include<bitset>
#include<fstream>
#include"PrepDES.h"

using std::bitset;

class DES
{
public:
	static std::ofstream& encrypt(std::ifstream&, bitset<56>, std::ofstream&);	//Шифрование файла
	static std::ofstream& decrypt(std::ifstream&, bitset<56>, std::ofstream&);	//Расшифрование файла
protected:
	bitset<64> encryptBlock(bitset<64>);	//Шифрование блока
	bitset<64> decryptBlock(bitset<64>);	//Расшифрование блока

};