#pragma once

#include <vector>
template <int sizeInBytes>
struct FileDescriptor
{
	std::vector<int> data;

	FileDescriptor()
	{
		data.assign(sizeInBytes, 0);
		data[0] = 1;
		data[1] = 0;
	}
	FileDescriptor(int const& fileLengthInBytes, std::vector<int> const& blockNumbers)
	{
		this->data.assign(blockNumbers.begin(), blockNumbers.end());
		this->data.insert(0, fileLengthInBytes);
	}
	bool isFree() {
		if ((data[0] != 0) && data[1] == 0) return true; //unreal situation
		else return false;
	}
};
