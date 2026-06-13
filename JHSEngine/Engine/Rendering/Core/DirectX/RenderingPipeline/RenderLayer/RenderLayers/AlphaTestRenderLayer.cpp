#include "AlphaTestRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FAlphaTestRenderLayer::FAlphaTestRenderLayer()
{
    renderPriority = 2450;
}

void FAlphaTestRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO(AlphaTest);
    
    Super::Draw(deltaTime);
}

void FAlphaTestRenderLayer::BuildShader()
{
}

void FAlphaTestRenderLayer::BuildPSO()
{
    Super::BuildPSO();
    //构建管线
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::AlphaTest);
}
