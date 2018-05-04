#pragma once

#include "Track.hpp"

struct Cylinder
{
	static const int tracksAmount = 2;
	static const int bytesAmount = tracksAmount * Track::bytesAmount;

	std::vector<Track> tracks;

	Cylinder()
	{
		tracks.reserve(tracksAmount);
		for (int i = 0; i < tracksAmount; i++)
		{
			tracks.emplace_back(Track());
		}
	}
};