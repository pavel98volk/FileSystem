#pragma once
#include"filesystem\FileSystem.hpp"

void shell()
{
	std::string command;
	while (true)
	{
		std::getline(std::cin, command);
		std::string temp = command.substr(0, 2);
		if (temp == "cr"){
			std::string name = command.substr(3, command.size() - 1);
		}
		else if (temp == "de"){
			std::string name = command.substr(3, command.size() - 1);
		}
		else if (temp == "op"){
			std::string name = command.substr(3, command.size() - 1);
		}
		else if (temp == "cl"){
			int index = std::stoi(command.substr(3, command.size() - 1));
		}
		else if (temp == "rd"){
			int i;
			int index;
			int count;
			std::string newTemp;
			for (i = 3; i < command.size(); i++){
				if (command.at(i) == ' ') break;
				newTemp += command.at(i);
			}
			index = stoi(newTemp);
			count = stoi(command.substr(++i, command.size() - 1));

		}
		else if (temp == "wr"){
			int i;
			int index;
			char mem_area;
			int count;
			std::string newTemp;
			for (i = 3; i < command.size(); i++){
				if (command.at(i) == ' ') break;
				newTemp += command.at(i);
			}
			index = stoi(newTemp);
			mem_area = command.at(++i);
			count = stoi(command.substr(++i, command.size() - 1));
		}
		else if (temp == "sk"){
			int i;
			int index;
			int pos;
			std::string newTemp;
			for (i = 3; i < command.size(); i++){
				if (command.at(i) == ' ') break;
				newTemp += command.at(i);
			}
			index = stoi(newTemp);
			pos = stoi(command.substr(++i, command.size() - 1));
		}
		else if (temp == "dr"){

		}
		else if (temp == "in"){
			std::string name = command.substr(3, command.size() - 1);
		}
		else if (temp == "sv"){
			std::string name = command.substr(3, command.size() - 1);
		}
		else break;

	}
}