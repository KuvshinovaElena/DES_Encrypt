#pragma once
#include<bitset>
#include "Tables.h"
namespace PrepDES {
	using std::bitset;
	using namespace Tables;
	bitset<> CiclShift(bitset<>, int);
	bitset<> IP(bitset<>);
	bitset<> FP(bitset<>);
	bitset<> E(bitset<>);
	bitset<> Feistel(bitset<>, bitset<>);
	bitset<> G (bitset<>);
	bitset<> createSubKey(bitset<>);

}