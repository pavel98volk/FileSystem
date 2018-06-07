#pragma once
#include"filesystem\FileSystem.hpp"
#include"filesystem\FileSystemTests.hpp"

void shell()
{
	LDisk disk;
	IOSystem io(disk);
	FileSystem<4, 4> *fs = new FileSystem<4, 4>(io);
	std::string command;
	while (true)
	{
		std::getline(std::cin, command);
		std::string temp = command.substr(0, 2);

		if (temp == "cr"){
			std::string name = "";
			try {
				name = command.substr(3, command.size() - 1);
			}
			catch (std::exception e){
				std::cout << "wrong input!\n";
			}
			if (name != "") {
				bool check = fs->createFile(name);
				if (check)
					std::cout << "File " << name << " created\n";
				else std::cout << "error occured\n";
			} 
		}
		else if (temp == "de"){
			std::string name = "";
			try {
				name = command.substr(3, command.size() - 1);
			}
			catch (std::exception e){
				std::cout << "wrong input!\n";
			}
			if (name != ""){
				bool check = fs->destroyFile(name);
				if (check)
					std::cout << "file " << name << " deleted\n";
				else std::cout << "error occured\n";
			}
		}
		else if (temp == "op"){
			std::string name = "";
			try {
				name = command.substr(3, command.size() - 1);
			}
			catch (std::exception e){
				std::cout << "wrong input!\n";
			}
			if (name != ""){
				int check = fs->openFile(name);
				if (check != -1)
					std::cout << "file " << name << " opened, index : " << check << std::endl;
				else std::cout << "can't open file\n";
			}
		}
		else if (temp == "cl"){
			int index = -1;
			try {
				index = std::stoi(command.substr(3, command.size() - 1));
			}
			catch (std::exception e) {
				std::cout << "invalid OFT index\n";
			}
			if (index != -1) {
				std::string check = fs->closeFile(index);
				if (check != "error")
					std::cout << "File " << check << " closed\n";
				else std::cout << "can't close this file\n";
			}
		}
		else if (temp == "rd"){
			int i;
			int index;
			int count;
			bool check = true;
			char *text = new char[100];
			for (int i = 0; i < 100; i++)
				text[i] = '\0';
			std::string newTemp;
			try {
				for (i = 3; i < command.size(); i++) {
					if (command.at(i) == ' ') break;
					newTemp += command.at(i);
				}
				index = stoi(newTemp);
				count = stoi(command.substr(++i, command.size() - 1));
			}
			catch (std::exception e) {
				std::cout << "wrong input!\n";
				check = false;
			}
			if (check) {
				if (fs->read(index, text, count))
					std::cout << count << " bytes read : " << text << std::endl;
				else std::cout << "error occured\n";
			}
		}
		else if (temp == "wr"){
			bool check = true;
			int i;
			int index;
			char* mem_area = new char[100];
			for (int i = 0; i < 100; i++)
				mem_area[i] = '\0';
			int count;
			std::string newTemp;
			try {
				for (i = 3; i < command.size(); i++) {
					if (command.at(i) == ' ') break;
					newTemp += command.at(i);
				}
				index = stoi(newTemp);
				int j = 0;
				while (++i < command.size()) {
					if (command.at(i) == ' ') break;
					mem_area[j] = command.at(i);
					j++;
				}
				count = stoi(command.substr(++i, command.size() - 1));
			}
			catch (std::exception e) {
				std::cout << "wrong input!\n";
				check = false;
			}
			if (check) {
				if (fs->write(index, mem_area, count))
					std::cout << count << " bytes written\n";
				else std::cout << "error occured\n";
			}
		}
		else if (temp == "sk"){
			bool check = true;
			int i;
			int index;
			int pos;
			std::string newTemp;
			try {
				for (i = 3; i < command.size(); i++) {
					if (command.at(i) == ' ') break;
					newTemp += command.at(i);
				}
				index = stoi(newTemp);
				pos = stoi(command.substr(++i, command.size() - 1));
			}
			catch (std::exception e) {
				std::cout << "wrong input!\n";
				check = false;
			}
			if (check) {
				if (fs->lseek(index, pos))
					std::cout << "current position is " << pos << std::endl;
				else std::cout << "error occured\n";
			}
		}
		else if (temp == "dr"){
			std::vector<std::pair<std::string, int>> r = fs->directory();
			for (std::pair<std::string, int> p : r) {
				std::cout << p.first << " " << p.second << " \n";
			}
		}
		else if (temp == "in"){
			//std::string name = command.substr(3, command.size() - 1);
			fs = new FileSystem<4, 4>(io);
			fs->clear();
			std::cout<<"created\n";
		}
		else if (temp == "sv"){
			fs->sync();
			std::cout << "You successfully executed \"sv\" command!\n";
		}
		else if (temp == "ex"){
			break;
		}
		else if (temp == "qq") {
			std::cout << "----------------------------------\n";
			std::cout<<fs->metadataToPrettyString();
			std::cout << "----------------------------------\n";
		}
		else if (temp == "fs") {
			std::string temp="";
			try {
				for (int i = 3; i < command.size(); i++) {
					if (command.at(i) == ' ') break;
					temp += command.at(i);
				}
			}
			catch (std::exception e) {
				std::cout << "specify the name of the restore file.\n";
			}
			if (temp != "") {
				fs->toFile(temp);
				std::cout << "restore file created.\n";
			}
		}
		else if (temp == "fr") {
			std::string temp = "";
			try {
				for (int i = 3; i < command.size(); i++) {
					if (command.at(i) == ' ') break;
					temp += command.at(i);
				}
			}
			catch (std::exception e) {
				std::cout << "wrong input!\n";
			}
			if(temp!="")
				fs->fromFile(temp);
		}
		else if (temp == "te") {
			FileSystemTests::allTests();
		}
		else {
			std::cout << "|||| command was not found !\n";
		}

	}
}