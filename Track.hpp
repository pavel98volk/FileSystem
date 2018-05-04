#pragma once

#include "Sector.hpp"

struct Track
{
	static const int sectorsAmount = 8;
	static const int bytesAmount = sectorsAmount * Sector::bytesAmount;
	
	std::vector<Sector> sectors;

	Track()
	{
		sectors.reserve(sectorsAmount);
		for (int i = 0; i < sectorsAmount; i++)
		{
			sectors.emplace_back(Sector());
		}
	}
};

