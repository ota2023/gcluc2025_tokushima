#include "DebugPrint.h"

DebugPrint* DebugPrint::ms_instance = nullptr;

DebugPrint::DebugPrint()
{
}

void DebugPrint::ClearInstance()
{
	if (ms_instance != nullptr)
	{
		delete ms_instance;
		ms_instance = nullptr;
	}
}

void DebugPrint::AddStr(std::string str)
{
	m_printStrs.push_back(str);
}

void DebugPrint::Print(const char* format, ...)
{
	if (ms_instance == nullptr)
	{
		ms_instance = new DebugPrint();
	}

	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	ms_instance->AddStr(buf);
}

void DebugPrint::Render()
{
	if (ms_instance == nullptr) return;

	static float startPosY = 32.0f;
	static float rowSize = 32.0f;
	std::vector<std::string>& strs = ms_instance->m_printStrs;
	for (int i = 0; i < strs.size(); i++)
	{
		FONT_T()->Draw
		(
			0, startPosY + rowSize * i,
			1.0f, 0.0f, 0.0f,
			strs[i].c_str()
		);
	}
	strs.clear();
}