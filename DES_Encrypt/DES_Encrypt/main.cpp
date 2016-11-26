#include "DES.h"
#include "PrepDES.h"
#include <fstream>
#include <bitset>

int main()
{
	char * key = "CryptDES";
	bitset<56> bitKey = PrepDES::converterKey(key);
	DES::encrypt("message.txt",bitKey,0);
	DES::encrypt("output.txt", bitKey, 1,"decrypt.txt");
	return 0;
}

