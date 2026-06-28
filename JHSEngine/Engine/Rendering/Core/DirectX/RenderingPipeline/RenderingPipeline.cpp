#include "RenderingPipeline.h"

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

    //构建常量描述堆
    geometryMap.BuildDescriptorHeap();

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
    
    renderLayerManager.PreDraw(deltaTime);

}

void FRenderingPipeline::Draw(float deltaTime)
{
    //主视口
    geometryMap.DrawViewport(deltaTime);
    //渲染灯光材质雾等
    geometryMap.Draw(deltaTime);
    //各类层级
    renderLayerManager.Draw(deltaTime);
    directXPipelineState.Draw(deltaTime);
}

void FRenderingPipeline::PostDraw(float deltaTime)
{
    geometryMap.PostDraw(deltaTime);
    renderLayerManager.PostDraw(deltaTime);
    directXPipelineState.PostDraw(deltaTime);
}
