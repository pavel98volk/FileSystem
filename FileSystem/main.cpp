#include <disk/LDisk.hpp>
#include <iosystem/IOSystem.hpp>
#include <filesystem/FileSystem.hpp>

//bugs: when there is need to extend a directory the length gets -128 after 128
void main() {
	LDisk disk;
	IOSystem io(disk);
	FileSystem<4, 4> fs(io);
	fs.clear();
	fs.createFile("one");
	fs.createFile("two");
	fs.createFile("three");
	fs.createFile("three");

	//fs.createFile("fla");
	std::cout << fs.metadataToPrettyString();
	system("pause");
	return;
}