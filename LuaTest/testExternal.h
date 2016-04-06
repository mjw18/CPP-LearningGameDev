#pragma once
#include <iostream>

class testExternal
{
public:
	testExternal();
	~testExternal();

	void addThings(int a, int b)
	{
		std::cout << (a + b) << std::endl;
	}

private:
	int m_name = 0;
};

struct ResourceManager{
	ResourceManager() : m_ResourceCount(0) {}

	void loadResource(const char* file)
	{
		++m_ResourceCount;
	}

	size_t getResourceCount() const{
		return m_ResourceCount;
	}

	size_t m_ResourceCount;
};