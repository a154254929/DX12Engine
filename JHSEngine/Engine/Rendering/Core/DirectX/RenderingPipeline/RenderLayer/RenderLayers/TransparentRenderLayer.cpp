#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
    renderPriority = 3000;
}

void FTransparentRenderLayer::Draw(float deltaTime)
{
    directXPipelineState->ResetPSO(Transparent);
    
    Super::Draw(deltaTime);
}

void FTransparentRenderLayer::BuildShader()
{
}

void FTransparentRenderLayer::BuildPSO()
{
    //构建管线
    directXPipelineState->SetFillMode(false);
    directXPipelineState->Build(EPipelineState::Transparent);
}
