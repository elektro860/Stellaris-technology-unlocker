#include "Parser.hpp"

const char c_fileTypeSeparator = (const char)".";
std::string getFileType(std::string p_fileName)
{
	std::string out = "";
	int i = 0;
	for (i = p_fileName.length() - 1; i >= 0; i++)
	{
		char l_char = p_fileName.at(i);
		if (l_char == c_fileTypeSeparator)
			break;
		out = l_char + out;
	}
	if (i == 0)
		out = "Folder";

	return out;
}
