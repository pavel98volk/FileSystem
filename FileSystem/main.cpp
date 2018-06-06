#include <disk/LDisk.hpp>
#include <iosystem/IOSystem.hpp>
#include <filesystem/FileSystem.hpp>
#include <filesystem/FileSystemTests.hpp>

//bugs: when there is need to extend a directory the length gets -128 after 128
void main() {
	//FileSystemTests::directoryTest();
	//FileSystemTests::createDestroyTest();  
	//FileSystemTests::OFTTest();
	FileSystemTests::saveFromFileTest();  
	return;
}