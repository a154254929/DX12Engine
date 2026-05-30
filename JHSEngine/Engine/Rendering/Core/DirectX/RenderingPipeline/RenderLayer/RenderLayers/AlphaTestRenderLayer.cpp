#include "AlphaTestRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
    renderPriority = 2450;
}

void FAlphaTestRenderLayer::BuildShader()
{
}

void FAlphaTestRenderLayer::BuildPSO()
{
    //构建管线
    directXPipelineState->Build(EPipelineState::WireFrame);
    
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::GrayModel);
}
