#pragma once
#include <vector>
#include <string>

class DebugPrint
{
public:
	DebugPrint();
	static void ClearInstance();
	static void Print(const char* format, ...);
	static void Render();

private:
	void AddStr(std::string str);

	static DebugPrint* ms_instance;
	std::vector<std::string> m_printStrs;
};