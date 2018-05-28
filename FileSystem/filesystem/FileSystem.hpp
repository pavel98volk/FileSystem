#pragma once
#include "iosystem\IOSystem.hpp"
#include "Metadata.hpp"

class FileSystem {
//data
	IOSystem *i;
	int blockCount; // const
	int blockLength; //const
	static const int MetadataBlockLen = 4;
	static const int DescriptorLen = 4; // amount of blocks in file descriptors;the overall descriptor size will be ||| (file_length)[1 byte] + file_descriptors[blocksPerFile bytes]; if file_length
	//file descriptor:
	//file length : 1 byte  //max_file_length = 256;
	//block numbers : blocksPerFile-1 ;
	//if the length is more than blocksPerFile, then the last block contains the pointer to another descriptor, that has no length in the first byte;
	Metadata* meta;
//functions
private:

public:
	FileSystem(IOSystem* i);


};
FileSystem::FileSystem(IOSystem*i) {
		this->i = i;
		blockCount = i->getBlockCount();
		blockLength = i->getBlockLength();
		this->meta = new Metadata(i, MetadataBlockLen);
		this->meta->clear();
	}
}


