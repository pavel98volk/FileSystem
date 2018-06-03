#include <disk/LDisk.hpp>
#include <iosystem/IOSystem.hpp>
#include <filesystem/FileSystem.hpp>


void main() {
	LDisk disk;
	IOSystem io(disk);
	FileSystem<4, 4> fs(io);
	fs.clear();
	fs.createFile("fle");
	std::cout << fs.metadataToPrettyString();
	system("pause");
	return;
}