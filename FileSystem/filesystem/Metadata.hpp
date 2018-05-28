#pragma once
#include "iosystem\IOSystem.hpp"
#include <stdexcept>
#include "filesystem\FileDescriptor.hpp"
#include <iostream>
#include <string>
//uses BIG ENDIAN to create ints from chars.
//represents bitmap and descriptors
union {
	int integer;
    char byte[4];
} int32char;

class Metadata {
	IOSystem* i;
	int k;//?
	static const int DESCRIPTOR_LEN = 4;
	int DescrArrLen;
	int bitsetCharSize;
	std::vector<char> buff;
public:
	Metadata(IOSystem* i, int k);
	void clear();
	
	FileDescriptor<DESCRIPTOR_LEN - 1> getDescriptor(int pos);
	void setDescriptor(int pos, FileDescriptor<DESCRIPTOR_LEN - 1> data);
	void setDescriptorFileLength(int pos, int len);
	std::string printAllBits();
	void loadFromString(std::string s);
	int findFreeBlock();
	void addFileToDirectory(int DescPos, std::string name);
	//additional
private:
	bool getBit(int pos);
	void setBit(int pos, bool data);
	char getChar(int pos);
	void setChar(int pos, char data);
	int  getInt(int pos);
	void setInt(int pos, int data);
};
//k - metadata length in blocks
Metadata::Metadata(IOSystem* i,int k) {
	if (k*(i->getBlockLength())<(i->getBlockCount)) throw std::invalid_argument("k is too small");
	
	this->i = i;
	this->k = k;
	this->bitsetCharSize = ((i->getBlockCount()) + 7) / 8;
	this->DescrArrLen = (i->getBlockLength() / 4 - ((bitsetCharSize + 3) / 4))/4;
	if (DescrArrLen < 11) std::cout << "max amount of files is less then 10 - exactly " << DescrArrLen;
}
bool Metadata::getBit(int pos) {
	return (getChar(pos / 8) >> (pos%8)) & 1;
}
char Metadata::getChar(int pos) {
	i->readBlock(pos / i->getBlockLength(), buff);
	pos %= i->getBlockLength();
	return buff.at(pos);
}
int Metadata::getInt(int pos) {
	int i = (getChar(pos * 4) << 24) | (getChar(pos * 4 + 1) << 16) | (getChar(pos * 4 + 2) << 8) | (getChar(pos * 4 + 3));
}
void Metadata::setInt(int pos, int data) {
	int32char.integer = data;
	setChar(pos * 4, (data >> 24) & 0xFF);
	setChar(pos * 4 +1, (data >> 16) & 0xFF);
	setChar(pos * 4 +2, (data >> 8) & 0xFF);
	setChar(pos * 4 +3, data  & 0xFF);
}
void Metadata::setBit(int pos,bool data) {
	char c = getChar(pos / 8);
	c |= 1 << (pos % 8);
	setChar(pos / 8, c);
}
void Metadata::setChar(int pos,char data) {
	i->readBlock(pos / i->getBlockLength(), buff);
	buff.at(pos%(i->getBlockLength())) = data;
	i->writeBlock(pos / i->getBlockLength(), buff);
}
void Metadata::clear() {
	for (int j = 0; j < bitsetCharSize; j++) {
		setChar(j, 0);
	}
	for (int j = 0; j < k; j++) {
		setChar(j, 1);
	}
	setDescriptorFileLength(0, 0); 
}
FileDescriptor<Metadata::DESCRIPTOR_LEN - 1> Metadata::getDescriptor(int pos) {
	if ((pos < 0) || (pos >= DescrArrLen)) throw std::invalid_argument("invalid");
	FileDescriptor<DESCRIPTOR_LEN - 1> d;
	d.fileLengthInBytes = getInt((bitsetCharSize + 3) / 4 + 4 * pos);
	for (int j = 0; j < (DESCRIPTOR_LEN - 1); j++)
		d.blockNumbers.at(j) = getInt((bitsetCharSize + 3) / 4 + 4 * pos+1 + j);
	return d;
}

void Metadata::setDescriptor(int pos , FileDescriptor<Metadata::DESCRIPTOR_LEN - 1> data) {
	if ((pos < 0) || (pos >= DescrArrLen)) throw std::invalid_argument("invalid ");
	setInt((bitsetCharSize + 3) / 4 + 4 * pos,data.fileLengthInBytes);
	for (int j = 0; j < (DESCRIPTOR_LEN - 1); j++)
		 setInt((bitsetCharSize + 3) / 4 + 4 * pos +1 + j,data.blockNumbers.at(j));
}
void Metadata::setDescriptorFileLength(int pos, int len) {
	if ((pos < 0) || (pos >= DescrArrLen)) throw std::invalid_argument("invalid ");
	setInt((bitsetCharSize + 3) / 4 + 4 * pos, len);
}

std::string Metadata::printAllBits() {
	std::string s = "";
	for (int j = 0; j < (i->getBlockLength()*k*8); i++) {
		s += getBit(j) ? "1" : "0";
	}
	return s;
}
void Metadata::loadFromString(std::string s) {
	for (int j = 0; j < (i->getBlockLength()*k * 8); i++) {
		setBit(j, (s[j] == '1') ? 1 : 0);
	}
}
int Metadata::findFreeBlock() {
	for (int j = 0; j < i->getBlockCount(); j++) {
		if (getBit(j) == 1) {
			setBit(j, 0);
			return j;
		}
	}
	std::cout << "out of memory";
	return -1;
}
//to finish
void Metadata::addFileToDirectory(int DescPos, std::string name) {
	if (getDescriptor(0).fileLengthInBytes / (i->getBlockLength()) < (getDescriptor(0).fileLengthInBytes + 8) / (i->getBlockLength())) {
		//findFreeBlock(0);
	}
}
