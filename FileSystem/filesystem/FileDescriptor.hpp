#pragma once

#include <vector>

struct FileDescriptor
{
	int fileLengthIntBytes;
	std::vector<int> blockNumbers;

	FileDescriptor()
	{
		fileLengthIntBytes = 0;
		blockNumbers.assign(3, -1);
	}

	FileDescriptor(int const& fileLengthInBytes, std::vector<int> const& blockNumbers)
	{
		this->fileLengthIntBytes = fileLengthInBytes;
		this->blockNumbers.assign(blockNumbers.begin(), blockNumbers.end());
	}
};
