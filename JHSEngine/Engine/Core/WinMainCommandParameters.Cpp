#include "WinMainCommandParameters.h"

FWinMainCommandParameters::FWinMainCommandParameters(
	HINSTANCE inHInstance
	, HINSTANCE inPrevInstance
	, PSTR inCmdLine
	, int inShowCmd
)
	:HInstance(inHInstance)
	, PrevInstance(inPrevInstance)
	, CmdLine(inCmdLine)
	, ShowCmd(inShowCmd)
{
}
