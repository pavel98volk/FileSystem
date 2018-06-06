#include <vector>
#include <iosystem/IOSystem.hpp>

struct OFTEntry
{
	std::vector<char> RWBuffer;
	int currentPosition;
	int fileDescriptorIndex;

	std::string fileName;

	bool empty;

	const int blockLength = 64; // Should be the same as IOSystem one

	OFTEntry()
	{
		/*RWBuffer.resize(blockLength);
		currentPosition = -1;
		fileDescriptorIndex = -1;*/
		clear();
	}

	void setFileName(std::string _fileName){ fileName = _fileName; }
	void clear() 
	{
		RWBuffer.clear();
		RWBuffer.resize(blockLength);
		currentPosition = -1;
		fileDescriptorIndex = -1;
		empty = true;
		fileName = "";
	}
};

struct OFT 
{
	std::vector<OFTEntry> entries;

	OFT()
	{
		clear();
	}

	void clear()
	{
		entries.clear();
		entries.reserve(4);
		for (int i = 0; i < 4; i++)
		{
			entries.emplace_back(OFTEntry());
		}
	}
};