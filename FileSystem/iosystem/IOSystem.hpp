#pragma once

#include <string>
#include <disk/LDisk.hpp>

class IOSystem
{
private:
	LDisk ldisk;

	const int blockLength = 64;
	const int blocksAmount = LDisk::bytesAmount / blockLength;
	const int blocksAmountInCylinder = Cylinder::bytesAmount / blockLength;
	const int blocksAmountInTrack = Track::bytesAmount / blockLength;
	const int blocksAmountInSector = Sector::bytesAmount / blockLength;

public:

	IOSystem(LDisk & ldisk)
	{
		this->ldisk = ldisk;
	}

private:

	std::vector<int> getBlockLocation(int const& blockNumber)
	{
		//Firstly searching cylinder number
		int cylinderNumber = -1; //undefined firstly
		for (int i = 1; i < LDisk::cylindersAmount; i++)
		{
			if (blockNumber < i * blocksAmountInCylinder)
			{
				cylinderNumber = i - 1;
				break;
			}
		}

		if (cylinderNumber == -1)
		{
			std::runtime_error("Cannot find cylinder number for block number" + blockNumber);
		}

		int blockNumberInCylinder = blockNumber % blocksAmountInCylinder;
		//Searching track number
		int trackNumber = -1;
		for (int i = 1; i < Cylinder::tracksAmount; i++)
		{
			if (blockNumberInCylinder < i * blocksAmountInTrack)
			{
				trackNumber = i - 1;
				break;
			}
		}

		if (trackNumber == -1)
		{
			std::runtime_error("Cannot find track number for block number" + blockNumber);
		}

		int blockNumberInTrack = blockNumberInCylinder % blocksAmountInTrack;

		//Searching actually block(sector) number
		int sectorNumber = -1;

		for (int i = 1; i < Track::sectorsAmount; i++)
		{
			if (blockNumberInTrack < i * blocksAmountInSector)
			{
				sectorNumber = i - 1;
				break;
			}
		}

		if (sectorNumber == -1)
		{
			std::runtime_error("Cannot find sector number for block number" + blockNumber);
		}

		std::vector<int> result;
		result.reserve(3);
		result.emplace_back(cylinderNumber);
		result.emplace_back(trackNumber);
		result.emplace_back(sectorNumber);

		return result;
	}

public:

	void readBlock(int const& blockNumber, std::vector<char> & destination)
	{
		if (blockNumber < 0 || blockNumber >= blocksAmount)
		{
			std::runtime_error("Block number " + std::to_string(blockNumber) + " should be >= 0 and < " + std::to_string(blocksAmount));
		}

		std::vector<int> blockLocation = getBlockLocation(blockNumber);

		for (int i = 0; i < blockLength; i++)
		{
			destination[i] = ldisk.cylinders[blockLocation[0]].tracks[blockLocation[1]].sectors[blockLocation[2]].bytes[i];
		}
	}

	void writeBlock(int const& blockNumber, std::vector<char> const& bytes)
	{
		if (blockNumber < 0 || blockNumber >= blocksAmount)
		{
			std::runtime_error("Block number " + std::to_string(blockNumber) + " should be >= 0 and < " + std::to_string(blocksAmount));
		}

		std::vector<int> blockLocation = getBlockLocation(blockNumber);

		for (int i = 0; i < blockLength; i++)
		{
			ldisk.cylinders[blockLocation[0]].tracks[blockLocation[1]].sectors[blockLocation[2]].bytes[i] = bytes[i];
		}
	}
};