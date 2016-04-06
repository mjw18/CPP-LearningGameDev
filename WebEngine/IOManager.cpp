#include "IOManager.h"
#include "Err.h"

#include <fstream>

namespace WebEngine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to end
		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = file.tellg();
		//put iterator back to the beginning
		file.seekg(0, std::ios::beg);

		//this is not important but Reduce file size by bytes of header if present
		fileSize -= file.tellg();

		//Read contents of file into buffer
		buffer.resize(fileSize);
		//pass char pointer from vector
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;

	}

	bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to end
		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = file.tellg();
		//put iterator back to the beginning
		file.seekg(0, std::ios::beg);

		//this is not important but Reduce file size by bytes of header if present
		fileSize -= file.tellg();

		//Read contents of file into buffer
		buffer.resize(fileSize);
		//pass char pointer from vector
		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;

	}
}