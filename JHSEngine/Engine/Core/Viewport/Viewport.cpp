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

void FViewport::ViewportInit()
{
    float aspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    XMMATRIX project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI, //以弧度为单位的自上而下的视场角
        aspectRatio,//宽高比
        1.0f,//近裁剪面
        10000.f//原剪裁面
    );

    XMStoreFloat4x4(&projectMatrix, project);
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
