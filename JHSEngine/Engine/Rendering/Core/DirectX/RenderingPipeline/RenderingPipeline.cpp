#include "RenderingPipeline.h"

#include "../../../../Component/Mesh/Core/MeshComponentType.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(const size_t inMeshHash, CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData)
{
    geometryMap.BuildMesh(inMeshHash, inMeshComponent, inMeshData);
}

void FRenderingPipeline::DuplicateMesh(CMeshComponent* inMeshComponent, const FRenderingData& meshRenderingData)
{
    geometryMap.DuplicateMesh(inMeshComponent, meshRenderingData);
}

bool FRenderingPipeline::FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType)
{
    return geometryMap.FindMeshRenderingData(inHash, meshData, inRenderLayerType);
}

void FRenderingPipeline::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
    dynamicCubeMap.UpdateCalculations(deltaTime, viewportInfo);
    
    geometryMap.UpdateCalculations(deltaTime, viewportInfo);
    
    renderLayerManager.UpdateCalculations(deltaTime, viewportInfo);
}


void FRenderingPipeline::BuildPipeline()
{
    //初始化GPS描述
    directXPipelineState.ResetGPSDesc();
    
    //渲染层级初始化
   renderLayerManager.Init(&geometryMap, &directXPipelineState);
    
    //统一排序渲染层级
   renderLayerManager.SortRenderLayer();
    
    //读取贴图纹理
    geometryMap.LoadTexture();
    
    //构建雾
    geometryMap.BuildFog();
    
    /**************************************/
    //构建动态Cubemap
    dynamicCubeMap.Init(
        &geometryMap,
        &directXPipelineState,
        &renderLayerManager
    );
    
    //构建动态Cubemap相机视口
    dynamicCubeMap.BuildViewPort(fvector_3d(4.5f, 13.5f, 0.f));
    
    //构建动态Cubemap深度图描述
    dynamicCubeMap.BuildDepthStencilDescriptor();
    
    //构建动态Cubemap深度图
    dynamicCubeMap.BuildDepthStencil();
    /**************************************/

    //构建根签名
    rootSignature.BuildRootSignature(
        geometryMap.GetDrawTexture2DResourcesNumber(),
        geometryMap.GetDrawTextureCubemapResourcesNumber()
    );
    directXPipelineState.BindRootSignature(rootSignature.GetRootSignature());
    
    //构建每个层级的shader
    //renderLayerManager.BuildShader();

    //构建模型
    geometryMap.Build();
    
    geometryMap.BuildDynamicReflectionMesh();

    //构建常量描述堆
    geometryMap.BuildDescriptorHeap();
    
    //构建动态Cubemap RT描述
    dynamicCubeMap.BuildRenderTargetDescriptor();

    //构建模型常量缓冲区
    geometryMap.BuildMeshConstantBuffer();

    //构建材质常量缓冲区
    geometryMap.BuildMaterialShaderResourceView();

    //构建光源常量缓冲区
    geometryMap.BuildLightConstantBuffer();

    //构建视口常量缓冲区
    geometryMap.BuildViewportConstantBuffer();

    //构建贴图
    geometryMap.BuildTextureConstantBuffer();

    //构建雾效
    geometryMap.BuildFogConstantBuffer();

    //构建PSO参数
    //directXPipelineState.BuildParam();
    
    //通过层级构建PSO
    renderLayerManager.BuildPSO();
}

void FRenderingPipeline::PreDraw(float deltaTime)
{
    directXPipelineState.PreDraw(deltaTime);
    
    rootSignature.PreDraw(deltaTime);
    
    geometryMap.PreDraw(deltaTime);
    
    //渲染灯光材质贴图等
    geometryMap.Draw(deltaTime);
    
    //动态Cubemap先渲染
    dynamicCubeMap.PreDraw(deltaTime);
    
    renderLayerManager.PreDraw(deltaTime);

}

void FRenderingPipeline::Draw(float deltaTime)
{
    //主视口
    geometryMap.DrawViewport(deltaTime);
    
    //渲染灯光材质雾等
    geometryMap.DrawTextureCubemap(deltaTime);
    //各类层级
    renderLayerManager.Draw(RENDERLAYER_BACKGROUND, deltaTime);
    renderLayerManager.Draw(RENDERLAYER_OPAQUE, deltaTime);
    renderLayerManager.Draw(RENDERLAYER_OPAQUE_REFLECTOR, deltaTime);
    renderLayerManager.Draw(RENDERLAYER_ALPHATEST, deltaTime);
    renderLayerManager.Draw(RENDERLAYER_TRANSPARENT, deltaTime);
    renderLayerManager.Draw(RENDERLAYER_POSTPROCESS, deltaTime);
    
    directXPipelineState.Draw(deltaTime);
}

void FRenderingPipeline::PostDraw(float deltaTime)
{
    geometryMap.PostDraw(deltaTime);
    renderLayerManager.PostDraw(deltaTime);
    directXPipelineState.PostDraw(deltaTime);
}
