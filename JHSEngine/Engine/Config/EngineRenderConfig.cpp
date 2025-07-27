#include "EngineRenderConfig.h"
FEngineRenderConfig* FEngineRenderConfig::renderConfig = nullptr;

FEngineRenderConfig::FEngineRenderConfig()
	: ScreenWidth(1280)
	, ScreenHeight(720)
	, RefreshRate(60)
	, SwapChainCount(2)
{
}

FEngineRenderConfig* FEngineRenderConfig::GetRenderConfig()
{
	if (!renderConfig)
	{
		renderConfig = new FEngineRenderConfig();
	}
	return renderConfig;
}

void FEngineRenderConfig::Destroy()
{
	if (renderConfig)
	{
		delete renderConfig;
		renderConfig = nullptr;
	}
}
