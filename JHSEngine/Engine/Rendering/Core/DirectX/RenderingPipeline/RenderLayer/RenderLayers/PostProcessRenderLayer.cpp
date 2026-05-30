#include "PostProcessRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FPostProcessRenderLayer::FPostProcessRenderLayer()
{
    renderPriority = 3500;
}

void FPostProcessRenderLayer::BuildShader()
{
}

void FPostProcessRenderLayer::BuildPSO()
{
    //构建管线
    directXPipelineState->Build(EPipelineState::WireFrame);
    
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::GrayModel);
}
