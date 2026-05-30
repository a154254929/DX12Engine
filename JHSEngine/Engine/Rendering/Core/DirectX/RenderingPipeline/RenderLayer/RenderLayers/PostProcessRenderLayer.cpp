#include "PostProcessRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FPostProcessRenderLayer::FPostProcessRenderLayer()
{
    renderPriority = 3500;
}

void FPostProcessRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO(PostProcess);
    Super::Draw(deltaTime);
}

void FPostProcessRenderLayer::BuildShader()
{
}

void FPostProcessRenderLayer::BuildPSO()
{
    //构建管线
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::PostProcess);
}
