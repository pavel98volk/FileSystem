#pragma once

#include <vector>
template <int n>
struct FileDescriptor
{
	int fileLengthInBytes;
	std::vector<int> blockNumbers;

	FileDescriptor()
	{
		fileLengthIntBytes = 0;
		blockNumbers.assign(n, -1);
	}
	FileDescriptor(int const& fileLengthInBytes, std::vector<int> const& blockNumbers)
	{
		this->fileLengthInBytes = fileLengthInBytes;
		this->blockNumbers.assign(blockNumbers.begin(), blockNumbers.end());
	}
	bool isFree() {
		if ((fileLengthInBytes != 0) && blockNumbers[0] == 0) return true; //unreal situation
		else return false;
	}
};
