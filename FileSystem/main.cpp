#include <disk/LDisk.hpp>
#include <iosystem/IOSystem.hpp>
#include <filesystem/FileSystem.hpp>

//bugs: when there is need to extend a directory the length gets -128 after 128
void main() {
	LDisk disk;
	IOSystem io(disk);
	FileSystem<4, 4> fs(io);
	fs.clear();
	
	/*fs.createFile("aah");
	fs.createFile("aat");
	fs.createFile("aay");
	fs.createFile("aai");
	fs.createFile("aal");
	fs.createFile("aaq");
	fs.createFile("baa");
	fs.createFile("bab");
	fs.createFile("bac");
	fs.createFile("bad");
	fs.createFile("bae");
	fs.createFile("baf");
	fs.createFile("bag");
	fs.createFile("bah");
	fs.createFile("bat");
	fs.createFile("bay");
	fs.createFile("bai");
	fs.createFile("bal");
	fs.createFile("baq");
	fs.createFile("bat");
	fs.createFile("dad");
	fs.createFile("dar");
	fs.createFile("daq");
	fs.createFile("caa");
	fs.createFile("cab");
	fs.createFile("cac");
	fs.createFile("cad");
	fs.createFile("cae");
	fs.createFile("caf");
	fs.createFile("cag");
	fs.createFile("cah");
	fs.createFile("cat");
	fs.createFile("cay");
	fs.createFile("cai");
	fs.createFile("cal");
	fs.createFile("caq");
	fs.createFile("cat");
	fs.createFile("ead");
	fs.createFile("ear");
	fs.createFile("eaa");
	fs.createFile("eab");
	fs.createFile("eac");
	fs.createFile("ead");
	fs.createFile("eae");
	fs.createFile("eaf");
	fs.createFile("eag");
	fs.createFile("eah");
	fs.createFile("eat");
	fs.createFile("eay");
	fs.createFile("eai");
	fs.createFile("eal");
	fs.createFile("eaq");
	fs.createFile("eat");
	*/
	
	std::cout << fs.metadataToPrettyString();
	system("pause");
	return;
}