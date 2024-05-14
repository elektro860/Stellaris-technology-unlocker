#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace filesystem = std::filesystem;

const char _separator = '.';
std::string getFileType(std::string p_fileName)
{
	std::string out = "";
	uint32_t i = p_fileName.length();
	for (i = i - 1; i > 0; i--)
	{
		char l_char = p_fileName[i];
		if (l_char == _separator)
			break;
		out = l_char + out;
	}
	if (i == 0)
		out = "Folder";
	return out;
}


class directory_file
{
public:
	std::string fileName;
	std::string fileType;
	std::string* content;
	uint32_t offset = 0;
	directory_file(const filesystem::directory_entry p_entry, uint32_t p_offset)
	{
		this->fileName = p_entry.path().filename().string();
		this->fileType = getFileType(this->fileName);
		this->offset = p_offset;
	}

	~directory_file()
	{
		if (this->content != nullptr)
			delete content;
		//std::cout << "Destroying directory file for file - " + this->fileName + "\n";
	}

};

void update_file_Content(directory_file* p_file, std::string p_path)
{
	std::cout << "Reading - " + p_file->fileName + "\n";
	std::ifstream openedFileInReadMode(p_path);
	if (!openedFileInReadMode)
		return;
	if (p_file->content != nullptr)
		delete p_file->content;

	p_file->content = new std::string((std::istreambuf_iterator<char>(openedFileInReadMode)), std::istreambuf_iterator<char>());
	openedFileInReadMode.close();
	return;
}

class directory_Viewer
{
public:
	std::string path;
	std::vector<directory_file*> files;

	~directory_Viewer()
	{
		for (int index = 0; index < this->files.size(); index++)
		{
			delete this->files[index];
		}
		this->files.clear();
	}

	void Refresh(bool p_update_content)
	{
		auto iter = this->files.begin();
		for (iter; iter < this->files.end(); iter++)
		{
			directory_file* file = *iter;
			delete file; // iter is pointing at the 
		}

		this->files.clear();

		for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
		{

			this->files.push_back(new directory_file(entry, (uint32_t)this->files.size()));
		}

		return;
	}

	void Update_Content(uint32_t offset, std::string type = "")
	{
		uint32_t size = this->files.size();
		if (offset > size)
			return;
		std::string p = this->path + "/";
		if (offset == size)
		{
			auto iter = this->files.begin();

			if (type == "") {
				for (iter; iter < this->files.end(); iter++)
				{
					directory_file* file = *iter;
					if (file->fileType == "Folder")
						continue;
					update_file_Content(file, p + file->fileName);
				}
				return;
			}
			for (iter; iter < this->files.end(); iter++)
			{
				directory_file* file = *iter;
				if (file->fileType != type)
					continue;
				update_file_Content(file, p + file->fileName);
			}
			return;
		}
		directory_file* file = this->files[offset];
		update_file_Content(file, p + file->fileName);
		return;
	}

	directory_Viewer(std::string p_targetPath)
	{
		this->path = p_targetPath;

		for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path))
		{
			directory_file* ptr = new directory_file(entry, (uint32_t)this->files.size());
			std::cout << "Assinging file - " + ptr->fileName + " Offset: " + std::to_string(ptr->offset) + " Type: ( " + ptr->fileType + " )\n";

			this->files.push_back(ptr);
		}
		std::cout << "created directory class\n";
	}
};

enum technologyParseState { parsing = 0, skipping, back_tracing };

std::vector<std::string> getTechnologiesFromFile(directory_file* p_file)
{
	std::string& fileContent = *p_file->content;

	uint64_t charIndex = 0;
	uint64_t stringSize = fileContent.length();
	int state = parsing;
	int level = 0;
	std::vector<std::string> out;

	while (charIndex < stringSize)
	{
		char current_char = fileContent[charIndex];
		bool is_not_equal = current_char != '\n';
		state *= is_not_equal; // sets the state to 0 if there's a new line
		current_char *= state != skipping; //should be branchless
		if (state == skipping)
			goto getTechsAdder;
		if (is_not_equal == 0)
			goto getTechsAdder;
		
		switch (current_char)
		{
		case ' ':
			break;
		case '	':
			break;
		case '=':
			if (level == 0)
			{
				state = back_tracing;
				uint64_t i = charIndex;
				std::string tech_name;
				while (true)
				{
					i--;
					char c = fileContent[i];
					if (c == '\n')
						break;
					
					if (c == ' ')
						continue;
					tech_name = c + tech_name;
					if (i == 0)
						break;
				}
				state = parsing;
				out.push_back(tech_name);
			}
			break;
		case '{':
			level++;
			break;
		case '}':
			level--;
			break;
		case '#':
			state = skipping;
			break;
		default:
			break;
		}
getTechsAdder:
		charIndex++;
	}

	return out;
}


