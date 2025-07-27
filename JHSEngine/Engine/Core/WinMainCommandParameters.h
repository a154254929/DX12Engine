#pragma once

#if defined(_WIN32)
#include "../EngineMinimal.h"
class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE inHInstance, HINSTANCE inPrevInstance, PSTR inCmdLine, int inShowCmd);

	HINSTANCE HInstance;
	HINSTANCE PrevInstance;
	PSTR CmdLine;
	int ShowCmd;
};
#elif 0
#endif