#pragma once

#include <string>
#include <vector>


namespace WebEngine {

	class IOManager
	{
	public:
		//fill vector with data
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
		//Fill string with data
		static bool readFileToBuffer(std::string filePath, std::string& buffer);

		template <class type> type writeClassToFile(type a);
		template <class type> type readFileToClass(type a);
	};

}