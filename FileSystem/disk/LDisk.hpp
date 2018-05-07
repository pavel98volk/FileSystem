#pragma once

#include "Cylinder.hpp"

struct LDisk
{
	static const int cylindersAmount = 4;
	static const int bytesAmount = cylindersAmount * Cylinder::bytesAmount;

	std::vector<Cylinder> cylinders;

	LDisk()
	{
		cylinders.reserve(cylindersAmount);
		for (int i = 0; i < cylindersAmount; i++)
		{
			cylinders.emplace_back(Cylinder());
		}
	}
};