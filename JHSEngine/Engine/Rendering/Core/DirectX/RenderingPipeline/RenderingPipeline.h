#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/DirectXPipelineState.h"
#include "RootSignature/DirectXRootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "RenderLayer/RenderLayerManager.h"

class CMeshComponent;
class FRenderingPipeline: public IDirectXDeviceInterface
{
public:
    FRenderingPipeline();

    void BuildMesh(const size_t inMeshHash, CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData);

    void DuplicateMesh(CMeshComponent* inMeshComponent, const FRenderingData& meshRenderingData);

    bool FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType = -1);

    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);

    void BuildPipeline();

    virtual void PreDraw(float deltaTime);
    virtual void Draw(float deltaTime);
    virtual void PostDraw(float deltaTime);
protected:
    FGeometryMap geometryMap;
    
    FRenderLayerManager renderLayerManager;

    FDirectXPipelineState directXPipelineState;
    FDirectXRootSignature rootSignature;
};