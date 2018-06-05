#pragma once
#include "iosystem\IOSystem.hpp"
#include <stdexcept>
#include "filesystem\FileDescriptor.hpp"
#include <iostream>
#include <string>
//uses BIG ENDIAN to create ints from chars.  
//represents bitmap and descriptors
//BitMap is using 0 for free space and 1 for used.
//The illegal combination used to determine the descriptor is free is 1 file length and 0 in the first ref_block 
// ( as if it was using the bitmap memory to store data).
// k- ammount of blocks spent on Metadaa
template <int k,int descriptorLength>
class Metadata {
	IOSystem& io;
	int descrArrLen;
	int bitsetCharSize;
	std::vector<char> buff;
public:
	Metadata(IOSystem& io);
	void clear();
	FileDescriptor<descriptorLength> getDescriptor(int pos);
	void setDescriptor(int pos, FileDescriptor<descriptorLength> data);
	void setDescriptorData(int descriptorPos, int dataPos, int data);
	std::string toBitString();
	std::string toPrettyString();
	void loadFromString(std::string s);
	int findFreeBlock();
	int findFreeDescriptor();
	void freeDescriptor();
	void freeBlock(int blockNum);
	void newFileToDir(std::string name);
	inline void freeDescriptor(int pos);
	//additional
private:
	
	void addDataToFile(int descriptorNum, std::vector<char>data);
	void addDataToFile(std::string name, std::vector<char>data);
	bool getBit(int pos);
	void setBit(int pos, bool data);
	unsigned char getChar(int pos);
	void setChar(int pos, char data);
	int  getInt(int pos);
	void setInt(int pos, int data);
};
//k - metadata length in blocks
template <int k, int descriptorLength>
Metadata<k,descriptorLength>::Metadata(IOSystem& io):io(io) {
	if (k*(io.getBlockLength())<(io.getBlockCount())) throw std::invalid_argument("k is too small");
	bitsetCharSize = ((io.getBlockCount()) + 7) / 8;
	descrArrLen = ((k*io.getBlockLength())/4 - ((bitsetCharSize + 3) / 4))/descriptorLength;
	if (descrArrLen < 11) std::cout << "max amount of files is less then 10 - exactly " << descrArrLen << std::endl;
	buff.resize(io.getBlockLength());
}

template <int k, int descriptorLength>
bool Metadata<k, descriptorLength>::getBit(int pos) {
	return (getChar(pos / 8) >> (pos%8)) & 1;
}

template <int k, int descriptorLength>
unsigned char Metadata<k, descriptorLength>::getChar(int pos) {
	io.readBlock(pos / io.getBlockLength(), buff);
	pos %= io.getBlockLength();
	return buff.at(pos);
}

template <int k, int descriptorLength>
int Metadata<k, descriptorLength>::getInt(int pos) {
	return (((int)getChar(pos * 4) << 24) | ((int)getChar(pos * 4 + 1) << 16) | ((int)getChar(pos * 4 + 2) << 8) | ((int)getChar(pos * 4 + 3)));
}
template <int k, int descriptorLength>

void Metadata<k, descriptorLength>::setInt(int pos, int data) {
	setChar(pos * 4, (data >> 24) & 0xFF);
	setChar(pos * 4 +1, (data >> 16) & 0xFF);
	setChar(pos * 4 +2, (data >> 8) & 0xFF);
	setChar(pos * 4 +3, data  & 0xFF);
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::setBit(int pos, bool data) {
	char c = getChar(pos / 8);
	if(data)
	c |= 1 << (pos % 8);
	else
	c &= ~(1 << (pos%8));
	setChar(pos / 8, c);
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::setChar(int pos, char data) {
	io.readBlock(pos / io.getBlockLength(), buff);
	buff.at(pos%(io.getBlockLength())) = data;
	io.writeBlock(pos / io.getBlockLength(), buff);
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::clear(){
	//1. clear the bitmap
	for(int j = k; j < io.getBlockCount(); j++) {
		setBit(j,1);
	}
	for (int j = 0; j < k; j++) {
		setBit(j,0);
	}
	//2. clean the directory
	setDescriptorData(0,0,0); 
	//3. free all other descriptors.
	for (int j = 1; j < descrArrLen; j++) {
		setDescriptorData(j, 0, 1);
		setDescriptorData(j, 1, 0);
	}

}

template <int k, int descriptorLength>
FileDescriptor<descriptorLength> Metadata<k, descriptorLength>::getDescriptor(int pos) {
	if ((pos < 0) || (pos >= descrArrLen)) throw std::invalid_argument("invalid");
	FileDescriptor<descriptorLength> d;
	for (int j = 0; j < (descriptorLength); j++) {
		d.data.at(j) = getInt((bitsetCharSize + 3) / 4 + descriptorLength * pos + j);
	}
	return d;
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::setDescriptor(int pos , FileDescriptor<descriptorLength> data) {
	if ((pos < 0) || (pos >= descrArrLen)) throw std::invalid_argument("invalid ");
	for (int j = 0; j < descriptorLength; j++)
		 setInt((bitsetCharSize + 3) / 4 + descriptorLength* pos + j, data.data.at(j));
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::setDescriptorData(int descriptorPos, int partPos, int value) {
	if ((descriptorPos < 0) || (descriptorPos >= descrArrLen)) throw std::invalid_argument("invalid descriptor");
	if ((partPos<0) || (partPos >= descriptorLength)) throw std::invalid_argument("invalid descriptor part");
	setInt((bitsetCharSize + 3) / 4 + descriptorLength * descriptorPos +partPos, value);
	value = getInt((bitsetCharSize + 3) / 4 + descriptorLength * descriptorPos + partPos);
	value++;
}


template <int k, int descriptorLength>
std::string Metadata<k, descriptorLength>::toBitString() {
	std::string s = "";
	for (int j = 0; j < (io.getBlockLength()*k*8); j++) {
		s += getBit(j) ? "1" : "0";
	} 
	return s;
}

template <int k, int descriptorLength>
std::string Metadata<k, descriptorLength>::toPrettyString() {
	std::string s = "Bitset: \n";
	for (int j = 0; j < ((bitsetCharSize+3)/4)*32; j++) {
		if (j < io.getBlockCount()) s += getBit(j) ? "1" : "0";
		else s += "X";
		if (j % (io.getBlockLength()*8) == (io.getBlockLength() * 8 -1)) s += "\n";
		else if (j % 8 == 7) s += " ";
	}

	s += "\n Descriptors:\n";
	for (int j = 0; j < descrArrLen; j++) {
		s += "|(number " + std::to_string(j) + ") ";
		for (int t = 0; t < descriptorLength; t++) {
			s += std::to_string(getDescriptor(j).data[t]) + " ";
		}
		 s+= "|\n";
	}
	s += "\n Free bytes in the end:\n";
	for (int j = (descrArrLen * 4 * descriptorLength+ bitsetCharSize); j < (io.getBlockLength()*k); j++) {
		s += "X";
		if (j % 64 == 63) s += "\n";
		else if (j % 4 == 3) s += " ";

	}
	return s;
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::loadFromString(std::string s) {
	for (int j = 0; j < (io.getBlockLength()*k * 8); i++) {
		setBit(j, (s[j] == '1') ? 1 : 0);
	}
}

template <int k, int descriptorLength>
int Metadata<k, descriptorLength>::findFreeBlock() {
	for (int j = 0; j < io.getBlockCount(); j++) {
		if (getBit(j) == 1) {
			setBit(j, 0);
			return j;
		}
	}
	std::cout << "out of memory";
	throw std::exception("no free blocks left");
}

template <int k, int descriptorLength>
int Metadata<k, descriptorLength>::findFreeDescriptor() {
	for (int j = 0; j < descrArrLen; j++) {
		FileDescriptor<descriptorLength>& d = getDescriptor(j);
		if (d.data[0] ==1 && d.data[1] ==0){
			d.data[0] = 0;
			setDescriptor(j, d);
			return j;
		}
	}
	std::cout << "out of memory";
	throw std::exception("no free blocks left");
}

template<int k, int descriptorLength>
inline void Metadata<k, descriptorLength>::freeBlock(int blockNum)
{
	setBit(blockNum, 1);
}

template <int k, int descriptorLength>
void Metadata<k, descriptorLength>::freeDescriptor(int pos) {
	setDescriptorData(pos, 0, 1);
	setDescriptorData(pos, 1, 0);
}

