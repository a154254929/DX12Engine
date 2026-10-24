#include "Viewport.h"
#include "../../Math/EngineMath.h"
#include "../../Config/EngineRenderConfig.h"

FViewport::FViewport()
    : viewMatrix(EngineMath::IdentityMatrix4x4())
    , projectMatrix(EngineMath::IdentityMatrix4x4())
{
    ResetViewport(
        FEngineRenderConfig::GetRenderConfig()->ScreenWidth
        , FEngineRenderConfig::GetRenderConfig()->ScreenHeight
    );
    ResetScissorRect(
        FEngineRenderConfig::GetRenderConfig()->ScreenWidth
        , FEngineRenderConfig::GetRenderConfig()->ScreenHeight
    );
}

void FViewport::ResetViewport(UINT inWidth, UINT inHeight)
{
    viewportInfo = {
        0.0f,
        0.0f,
        (float)inWidth,
        (float)inHeight,
        0.0f,
        1.0f
    };
}

void FViewport::ResetScissorRect(UINT inWidth, UINT inHeight)
{
    viewportRect = {
        0,
        0,
        (LONG)inWidth,
        (LONG)inHeight
    };
}
