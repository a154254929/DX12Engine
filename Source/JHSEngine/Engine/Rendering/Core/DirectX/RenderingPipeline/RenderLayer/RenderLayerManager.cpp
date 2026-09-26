#include "RenderLayerManager.h"
#include "RenderLayers/AlphaTestRenderLayer.h"
#include "RenderLayers/OpaqueRenderLayer.h"
#include "RenderLayers/PostProcessRenderLayer.h"
#include "RenderLayers/TransparentRenderLayer.h"
#include "RenderLayers/BackGroundRenderLayer.h"
#include "RenderLayers/OpaqueReflectorRenderLayer.h"
#include "RenderLayers/OpaqueShadowRenderLayer.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "Engine/Component/Mesh/Core/MeshComponent.h"
#include "Engine/Component/Mesh/Core/MeshComponentType.h"
#include "Engine/Core/World.h"
#include "Engine/Mesh/Core/Mesh.h"
#include "Engine/Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"
#if EDITOR_ENGINE
#include "RenderLayers/OperationHandleRenderLayer.h"
#include "RenderLayers/SelectRenderLayer.h"
#endif

std::vector<shared_ptr<FRenderLayer>> FRenderLayerManager::renderLayers;

FRenderLayerManager::FRenderLayerManager()
{
    renderLayers.clear();
    
    CreateRenderLayer<FAlphaTestRenderLayer>();
    CreateRenderLayer<FOpaqueRenderLayer>();
    CreateRenderLayer<FPostProcessRenderLayer>();
    CreateRenderLayer<FTransparentRenderLayer>();
    CreateRenderLayer<FBackGroundRenderLayer>();
    CreateRenderLayer<FOpaqueReflectorRenderLayer>();
    CreateRenderLayer<FOpaqueShadowRenderLayer>();
#if EDITOR_ENGINE
    CreateRenderLayer<FSelectRenderLayer>();
    CreateRenderLayer<FOperationHandleRenderLayer>();
#endif
}

FRenderLayerManager::~FRenderLayerManager()
{
    renderLayers.clear();
}

void FRenderLayerManager::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    for (auto& tmp : renderLayers)
    {
        tmp->Init(inGeometryMap, inDirectXPipelineState);
    }
}

void FRenderLayerManager::PreDraw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->PreDraw(deltaTime);
    }
}

void FRenderLayerManager::Draw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->Draw(deltaTime);
    }
}

void FRenderLayerManager::PostDraw(float deltaTime)
{
    for (auto& tmp : renderLayers)
    {
        tmp->PostDraw(deltaTime);
    }
}

void FRenderLayerManager::Draw(int inLayer, float deltaTime)
{
    
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->Draw(deltaTime);
    }
}

void FRenderLayerManager::FindObjDraw(int inLayer, float deltaTime, const CMeshComponent* inMeshComponent)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->FindObjDraw(deltaTime, inMeshComponent);
    }
}

void FRenderLayerManager::BuildPSO()
{
    for (auto& tmp : renderLayers)
    {
        tmp->BuildPSO();
    }
}

void FRenderLayerManager::HighlightDisplayObject(GActorObject* inActorObject)
{
    if (GMesh* inMeshActor = dynamic_cast<GMesh*>(inActorObject))
    {
        CMeshComponent* inMeshComponent = inMeshActor->GetMeshComponent();
        FGeometry::FindRenderingDatas([&](std::shared_ptr<FRenderingData> renderingData)
        {
            if (renderingData->meshComp == inMeshComponent)
            {
                HighlightDisplayObject(renderingData);
                return EFindValueType::EFindValueType_Complete;
            }
            return EFindValueType::EFindValueType_In_progress;
        });
    }
}

extern int actorSelectId;
void FRenderLayerManager::HighlightDisplayObject(std::weak_ptr<FRenderingData> inRenderingData)
{
    Clear(EMeshRenderLayerType::RENDERLAYER_OPAQUE_SELECT);
    Add(EMeshRenderLayerType::RENDERLAYER_OPAQUE_SELECT, inRenderingData);
    
#if EDITOR_ENGINE
    //记录index
    GActorObject* inActorObject = dynamic_cast<GActorObject*>(inRenderingData.lock()->meshComp->GetOwner());
    if (inActorObject)
    {
        for (int i = 0; i < GetWorld()->GetAllActors().size(); ++i)
        {
            if (GetWorld()->GetAllActors()[i] == inActorObject)
            {
                actorSelectId = i;
                break;
            }
        }
    }
#endif
}

void FRenderLayerManager::HighlightDisplayObject(CComponent* inComponent)
{
    if (CMeshComponent* inMeshComponent = dynamic_cast<CMeshComponent*>(inComponent))
    {
        FGeometry::FindRenderingDatas([&](std::shared_ptr<FRenderingData> renderingData)
        {
            if (renderingData->meshComp == inMeshComponent)
            {
                HighlightDisplayObject(renderingData);
                return EFindValueType::EFindValueType_Complete;
            }
            return EFindValueType::EFindValueType_In_progress;
        });
    }
}

void FRenderLayerManager::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    for (auto& tmp : renderLayers)
    {
        tmp->UpdateCalculations(deltaTime, viewportInfo);
    }
}

void FRenderLayerManager::BuildShader()
{
    for (auto& tmp : renderLayers)
    {
        tmp->BuildShader();
    }
}

void FRenderLayerManager::SortRenderLayer()
{
    auto compRenderLayer = [&](const std::shared_ptr<FRenderLayer> renderLayerA, std::shared_ptr<FRenderLayer> renderLayerB)
    {
        return renderLayerA->GetRenderLayerPriority() < renderLayerB->GetRenderLayerPriority();
    };
    std::sort(renderLayers.begin(), renderLayers.end(), compRenderLayer);
}

void FRenderLayerManager::ResetPSO(int inLayer)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->ResetPSO();
    }
}

void FRenderLayerManager::ResetPSO(int inLayer, EPipelineState inPipelineState)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->ResetPSO(inPipelineState);
    }
}

void FRenderLayerManager::DrawMesh(float deltaTime, int inLayer, ERenderingConditions inRenderingConditions)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->DrawMesh(deltaTime, inRenderingConditions);
    }
}

std::shared_ptr<FRenderLayer> FRenderLayerManager::FindByRenderLayer(int inRenderLayerType)
{
    for (auto& tmp : renderLayers)
    {
        if (tmp->GetRenderLayerType() == inRenderLayerType)
            return tmp;
    }
    return NULL;
}

void FRenderLayerManager::Add(int inLayer, std::weak_ptr<FRenderingData> inWeakRenderingData)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->Add(inWeakRenderingData);
    }
}

void FRenderLayerManager::Remove(int inLayer, std::weak_ptr<FRenderingData> inWeakRenderingData)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->Remove(inWeakRenderingData);
    }
}

void FRenderLayerManager::Clear(int inLayer)
{
    if (auto inRenderLayer = FindByRenderLayer(inLayer))
    {
        inRenderLayer->Clear();
    }
}
