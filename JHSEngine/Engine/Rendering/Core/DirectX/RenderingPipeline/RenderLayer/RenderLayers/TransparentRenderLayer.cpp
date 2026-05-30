#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
    renderPriority = 3000;
}

void FTransparentRenderLayer::BuildShader()
{
}

void FTransparentRenderLayer::BuildPSO()
{
    //构建管线
    directXPipelineState->Build(EPipelineState::WireFrame);
    
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::GrayModel);
}
