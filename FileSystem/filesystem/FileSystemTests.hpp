
#pragma once;
#include ".\FileSystem.hpp"
#include "..\disk\LDisk.hpp"
#include "..\iosystem\IOSystem.hpp"
#include <vector>

class FileSystemTests: private FileSystem<4,4> {
public:
	static void allTests() {
		std::cout << "------------------------------\n";
			std::cout << "-closeNotOpened-----------"<< closeNotOpened();
	}
	static bool saveToFileTest() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<16, 4> fs(io);
		fs.clear();
		fs.createFile("aah");
		fs.createFile("aat");
		fs.createFile("aay");
		fs.createFile("aai");
		fs.toFile("test.txt");

		std::cout << fs.metadataToPrettyString();

		system("pause");
		return true;
	}
	static bool saveFromFileTest() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<16, 4> fs(io);
		fs.clear();
		fs.fromFile("test.txt");
		std::cout << fs.metadataToPrettyString();
		system("pause");
		return true;
	}
	static bool createDestroyTest() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<16, 4> fs(io);
		fs.clear();

		fs.createFile("asga");
		fs.createFile("aata");
		fs.createFile("aaya");
		fs.createFile("aaia");
		fs.destroyFile("aata");
		fs.destroyFile("aata");
		fs.createFile("alka"); 
		fs.createFile("asda");
		/*fs.createFile("aal");
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
		fs.createFile("eat");*/
		std::cout << fs.metadataToPrettyString();
		return true;
	}
	static bool OFTTest() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<16, 4> fs(io);
		fs.clear();
		fs.createFile("aah");
		fs.createFile("aat");
		fs.createFile("aay");
		fs.createFile("aai")	;
		
		int index = fs.openFile("aay");
		char* data = new char[100];
		for (int i = 0; i < 100; i++) {
			data[i] = 'a';
		}
		std::cout<<fs.write(index, data, 60) << '\n';
		std::cout<<fs.lseek(index, 0) << '\n';
		for (int i = 0; i < 100; i++) {
			data[i] = 'b';
		}
		std::cout << fs.write(index, data, 20) << '\n';
		std::cout << fs.lseek(index, 0) << '\n';
		std::cout<<fs.read(index, data, 40) << '\n';
		data[99] = '\0';
		std::cout << data << '\n';
		std::cout<<fs.closeFile(index) << '\n';

		
		return true;
	}
	static bool directoryTest() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<4, 4> fs(io);
		fs.clear();

		fs.createFile("aah");
		fs.createFile("aat");
		fs.createFile("aay");
		fs.createFile("aai");
		fs.createFile("aal");
		fs.createFile("aaq");
		fs.createFile("baa");
		
		std::vector<std::pair<std::string,int>> r = fs.directory();
		for (std::pair<std::string, int> p : r) {
			std::cout << p.first << " " << p.second << " \n";
		}
		return true;
	}
	static bool createWithSameName() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<4, 4> fs(io);
		fs.clear();

		fs.createFile("aah");
		fs.createFile("aat");
		fs.createFile("aay"); 
		fs.createFile("aah");
		fs.createFile("aat");
		fs.createFile("aay");

		std::vector<std::pair<std::string, int>> r = fs.directory();
		for (std::pair<std::string, int> p : r) {
			std::cout << p.first << " " << p.second << " \n";
		}
		return true;
	}
	static bool openOpened() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<4, 4> fs(io);
		fs.clear();

		fs.createFile("aah");
		fs.openFile("aah");
		fs.openFile("aah");
		return true;
	}
	static bool closeNotOpened() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<4, 4> fs(io);
		fs.clear();
		std::string check = fs.closeFile(0);
		if (check != "error")
			return false;
		else return true;

	}
	static bool deleteDeleted() {
		LDisk disk;
		IOSystem io(disk);
		FileSystem<4, 4> fs(io);
		fs.clear();
		bool check = fs.destroyFile("ddd");
		if (check)
			std::cout << "file ddd deleted\n";
		else std::cout << "error occured\n";
		return true;
	}

};