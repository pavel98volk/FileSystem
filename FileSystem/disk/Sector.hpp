#pragma once
#include <vector>

struct Sector
{
	static const int bytesAmount = 64;

	std::vector<char> bytes;

	Sector()
	{
		bytes.resize(bytesAmount);
	}
};