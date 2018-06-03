#pragma once
#include "iosystem\IOSystem.hpp"
#include "Metadata.hpp"
template <int k, int descriptorLength>
class FileSystem {
//data
	 IOSystem &io;
	 Metadata<k, descriptorLength> meta;
	std::vector<char> buff;

public:
	std::string metadataToPrettyString();
	FileSystem(IOSystem& io);
	void clear();

	bool rewriteBlock(int fileDescrNum, int blockNumber, std::vector<char>& data);
	bool readBlock(int fileDescrNum, int blockNumber, std::vector<char>& data);
	bool addBlock(int fileDescrNum, std::vector<char>& data);
	bool removeLastBlock(int fileDescrNum);
	bool createFile(std::string name);
private:
	int getDescriptorByFileName(std::string name);
	
};

template<int k, int descriptorLength>
std::string FileSystem<k, descriptorLength>::metadataToPrettyString()
{	
	std::string s = "\n";
	s += "DIRECTORY\n";
	s += "todo...\n";
	FileDescriptor<descriptorLength> d = meta.getDescriptor(0);
	int len = d.data[0];
	int blockNum = (len + (io.getBlockLength() - 1)) / io.getBlockLength();
	
	for (int i = 0; i < (len / 2); i++) {
		int blockNum = (len + (io.getBlockLength() - 1)) / io.getBlockLength();
	}
	return meta.toPrettyString() + s;
}

template <int k, int descriptorLength>
FileSystem<k, descriptorLength>::FileSystem(IOSystem &io): io(io),meta(Metadata<k,descriptorLength>(io)){

		buff.resize(io.getBlockLength());
}
template<int k, int descriptorLength>
inline void FileSystem<k, descriptorLength>::clear(){
	meta.clear();
}

template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::createFile(std::string name){
	//1) find free descriptor
	int descriptorNum = meta.findFreeDescriptor();
	FileDescriptor<descriptorLength> dir = meta.getDescriptor(0);
	int len = dir.data[0];
	int blockNum = (len + (io.getBlockLength() - 1)) / io.getBlockLength();
	readBlock(0, blockNum - 1, buff);
	len %= io.getBlockLength();
	int i = 0;
	while (len < 64 && i<4) {
		buff[len] = name[i];
		len++;
		i++;
	}
	while (len < 64 && i<8) {
		buff[len] = ((descriptorNum) >> (24 - 8 * (i - 4)) & 0xFF);
		len++;
		i++;
	}
	rewriteBlock(0, blockNum - 1, buff);
	//if(file is bigger now) make it bigger.
	if ((dir.data[0] + 8) / io.getBlockLength() > dir.data[0] / io.getBlockLength()) {
		for (int j = 0; i < 8; i++, j++) {
			if (i < 4) {
				buff[j] = name[i];
			}
			else {
				buff[j] = (descriptorNum) >> ((24 - 8 * (i - 4)) & 0xFF);
			}
		}
		//addBlock(0, buff);
	}
	meta.setDescriptorData(0, 0, dir.data[0] + 8);
	return true;
}

template<int k, int descriptorLength>
inline int FileSystem<k, descriptorLength>::getDescriptorByFileName(std::string name)
{
	return 0;
}

template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::rewriteBlock(int fileDescrNum, int blockNumber, std::vector<char>& data) {
	FileDescriptor<descriptorLength> d = meta.getDescriptor(fileDescrNum);
	int len = d.data[0];
	int blockNum = (len + (io.getBlockLength()-1)) / io.getBlockLength();
	if (blockNum == 0 || blockNum <=blockNumber) return false;
	if (blockNumber <= (descriptorLength - 1)) {
		io.writeBlock(d.data[blockNum + 1], data);
	}
	else {
		while ((blockNumber > (descriptorLength - 1))|| (blockNumber ==(descriptorLength-1))&& (blockNum> (descriptorLength - 1))) {
			d = meta.getDescriptor(d.data[descriptorLength - 1]);
			blockNumber -= (descriptorLength - 1);
			blockNum -= (descriptorLength - 1);
		}
		io.writeBlock(d.data[blockNum], data);
	}
	return true;
}

template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::readBlock(int fileDescrNum, int blockNumber, std::vector<char>& data) {
	FileDescriptor<descriptorLength> d = meta.getDescriptor(fileDescrNum);
	int len = d.data[0];
	int blockNum = (len + (io.getBlockLength() - 1)) / io.getBlockLength();
	if (blockNum == 0 || blockNum <= blockNumber) return false;
	if (blockNumber <= (descriptorLength - 1)) {
		io.readBlock(d.data[blockNum + 1], data);
	}
	else {
		while ((blockNumber > (descriptorLength - 1)) || (blockNumber == (descriptorLength - 1)) && (blockNum> (descriptorLength - 1))) {
			d = meta.getDescriptor(d.data[descriptorLength - 1]);
			blockNumber -= (descriptorLength - 1);
			blockNum -= (descriptorLength - 1);
		}
		io.readBlock(d.data[blockNum], data);
	}
	return true;
}

template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::addBlock(int fileDescrNum, std::vector<char>&data) {
	int dataBlock = meta.findFreeBlock();
	FileDescriptor<descriptorLngth> d = meta.getDescriptor(fileDescrNum);
	int len = d.data[0];
	if (len == 0) return false;
	int blockNum = (len + (io.getBlockLength()-1)) / io.getBlockLength();
	if (blockNum < (descriptorLength - 1)) {
		d.data[blockNum + 2] = data;
	}
	else if (blockNum % (descriptorLength - 1) == 0) {
		while (blockNum != (descriptorLength - 1)) {
			d = meta.getDescriptor(d.data[descriptorLength - 1]);
			blockNum -= (descriptorLength - 1);
		}
		int tData = d.data[descriptorLength - 1];
		d.data[descriptorLength - 1] = meta.findFreeDescriptor();
		d = meta.getDescriptor(d.data[descriptorLength - 1]);
		d.data[0] = tData;
		d.data[1] = data;
	}
	else {
		while (blockNum  >(descriptorLength - 2)) {
			d = meta.getDescriptor(d.data[descriptorLength - 1]);
			blockNum -= (descriptorLength - 1);
		}
		d.data[blockNum + 1] = data;
	}
	return true;
}

template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::removeLastBlock(int fileDescrNum) {
 	FileDescriptor<descriptorLngth> d = meta.getDescriptor(fileDescrNum);
	int len = d.data[0];
	if (len == 0) return false;
	int blockNum = (len + (io.getBlockLength()-1)) / io.getBlockLength();
	if (blockNum < descriptorLength) {
		meta.freeBlock(d.data[blockNum - 1]);
	}
	else if (blockNum % (descriptorLength - 1) == 1) {
		while (blockNum != (descriptorLength - 1)) {
			d = meta.getDescriptor(d.data[descriptorLength-1]);
			blockNum -= (descriptorLength - 1);
		}
		FileDescriptor<descriptorLngth> d1 = meta.getDescriptor(d.data[(descriptorLength - 1)]);
		int dataToTransfer = d1.data[0];
		meta.freeBlock(d1.data[1]);
		meta.freeDescriptor(d.data[descriptorLength - 1]);
		d.data[descriptorLength - 1] = dataToTransfer;
	} else {
		while (blockNum  >(descriptorLength - 1)) {
			d = meta.getDescriptor(d.data[(descriptorLength - 1)]);
		    blockNum -= (descriptorLength - 1);
		}
		meta.freeBlock(d.data[blockNum]);
	}
	return true; 
}
/*
template<int k, int descriptorLength>
bool FileSystem<k, descriptorLength>::updateFile(int descriptorNum, std::vector<char> data){
		FileDescriptor<descriptorLength> d = meta.getFileDescriptor(descriptorNum);
		int blockNum = (d.data[0] + io.getBlockLength() - 1) / io.getBlockLength();
	//if(file is bigger now) make it bigger.
		if (blockNum != ((d.data[0] + 8 + io.getBlockLength()-1) / io.getBlockLength())) {

		}
	//if(file is smaller now) shrink it again
	// write Contents.

		return true;
} 
*/

