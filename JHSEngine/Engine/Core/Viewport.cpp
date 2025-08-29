#include "Viewport.h"
#include "../Mesh/Core/ObjectTransformation.h"
#include "../Config/EngineRenderConfig.h"

FViewport::FViewport()
	: viewMatrix(FObjectTransformation::IdentityMatrix4x4())
	, projectMatrix(FObjectTransformation::IdentityMatrix4x4())
{

}

void FViewport::ViewportInit()
{
    float aspectRatio = (float)FEngineRenderConfig::GetRenderConfig()->ScreenWidth / (float)FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
    XMMATRIX project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI, //以弧度为单位的自上而下的视场角
        aspectRatio,//宽高比
        1.0f,//近裁剪面
        1000.f//原剪裁面
    );

    XMStoreFloat4x4(&projectMatrix, project);
}