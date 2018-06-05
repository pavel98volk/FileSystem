#include <vector>
#include <iosystem/IOSystem.hpp>

struct OFTEntry
{
	std::vector<char> RWBuffer;
	int currentPosition;
	int fileDescriptorIndex;

	int curFileBlock;

	bool empty = true;

	const int blockLength = 64; // Should be the same as IOSystem one

	OFTEntry()
	{
		RWBuffer.resize(blockLength);
		currentPosition = -1;
		fileDescriptorIndex = -1;
		curFileBlock = -1;
	}
};

struct OFT 
{
	std::vector<OFTEntry> entries;

	OFT()
	{
		entries.resize(4);
	}
};