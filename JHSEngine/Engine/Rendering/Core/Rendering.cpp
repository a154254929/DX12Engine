#include "Rendering.h"
#ifdef _WIN32
#include "../../Platform/Windows/WindowsEngine.h"
#endif

vector<IRenderingInterface*> IRenderingInterface::renderingInterfaces;

IRenderingInterface::IRenderingInterface()
{
	renderingInterfaces.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (auto iter = renderingInterfaces.begin(); iter != renderingInterfaces.end(); ++iter)
	{
		if (*iter == this)
		{
			renderingInterfaces.erase(iter);
			break;
		}
	}
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float deltaTime)
{
	//重置命令列表
	//ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));
}

void IRenderingInterface::Draw(float deltaTime)
{
}

void IRenderingInterface::PostDraw(float deltaTime)
{
}
