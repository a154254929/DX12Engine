#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferView.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"


class CFogComponent;
struct FRenderingTexture;
class CMaterial;

class FGeometry : public IDirectXDeviceInterface_Struct
{
    friend struct FGeometryMap;
public:
    bool IsRenderingDataExistence(CMeshComponent* inKey);
    
    void BuildMesh(const size_t inMeshHash, CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData, int inGeometryKey);

    void DuplicateMesh(CMeshComponent* inMeshComponent, const FRenderingData& meshRenderingData, int inGeometryKey);

    bool FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType = -1);

    //构建模型
    void Build();

    /*后面会有变化*/
    UINT GetDrawObjectNumber() const;

    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
    ComPtr<ID3DBlob> cpuVertexBufferPtr;
    ComPtr<ID3DBlob> cpuIndexBufferPtr;

    ComPtr<ID3D12Resource> gpuVertexBufferPtr;
    ComPtr<ID3D12Resource> gpuIndexBufferPtr;

    ComPtr<ID3D12Resource> vertexBufferTmpPtr;
    ComPtr<ID3D12Resource> indexBufferTmpPtr;

    FMeshRenderingData meshRenderingData;
};

class FGeometryMap : public IDirectXDeviceInterface
{
    friend class FRenderLayer;
public:
    FGeometryMap();
    ~FGeometryMap();

    void PreDraw(float deltaTime);
    void Draw(float deltaTime);
    void PostDraw(float deltaTime);

    virtual void UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo);
    
    virtual void UpdateMaterialShaderResourceView(float deltaTime, const FViewportInfo viewportInfo);

    void BuildMesh(const size_t inMeshHash, CMeshComponent* inMeshComponent, const FMeshRenderingData& inMeshData);

    void DuplicateMesh(CMeshComponent* inMeshComponent, const FRenderingData& meshRenderingData);

    bool FindMeshRenderingData(const size_t& inHash, FRenderingData& meshData, int inRenderLayerType = -1);

    void Build();
    //描述堆
    void BuildDescriptorHeap();

    //常量缓冲区
    void BuildMeshConstantBuffer();

    //构建材质常量缓冲区
    void BuildMaterialShaderResourceView();

    //构建视口常量缓冲区
    void BuildViewportConstantBuffer();

    //构建光源常量缓冲区
    void BuildLightConstantBuffer();

    //构建雾常量缓冲区
    void BuildFogConstantBuffer();

    //构建贴图SRV视图
    void BuildTextureConstantBuffer();

    /*后面会有变化*/
    UINT GetDrawMeshObjectNumber();

    /*后面会有变化*/
    UINT GetDrawMaterialObjectNumber();

    /*后面会有变化*/
    UINT GetDrawLightObjectNumber();

    /*后面会有变化*/
    UINT GetDrawTexture2DResourcesNumber();
    
    UINT GetDrawTextureCubemapResourcesNumber();
    
public:
    std::unique_ptr<FRenderingTexture> *FindRenderingTextureByName(const std::string &inKey);

public:
    void DrawViewport(float deltaTime);

    void DrawLight(float deltaTime);

    void DrawMesh(float deltaTime);

    void DrawFog(float deltaTime);
    
    void DrawMaterial(float deltaTime);
    
    void DrawTexture(float deltaTime);
    
    void LoadTexture();
    
    void BuildFog();
    
public:
    bool IsFogOn();
public:
    ID3D12DescriptorHeap* GetHeap() const { return descriptorHeap.GetHeap(); }
protected:
    map<int, FGeometry> geometrys;
    FDirectXDescriptorHeap descriptorHeap;

    FConstantBufferView meshConstantBufferView;
    FConstantBufferView materialConstantBufferView;
    FConstantBufferView viewportConstantBufferView;
    FConstantBufferView lightConstantBufferView;
    FConstantBufferView fogConstantBufferView;
    
    shared_ptr<class FRenderingTextureResourcesUpdate> renderingTexture2DResourcesUpdate;
    shared_ptr<class FRenderingTextureResourcesUpdate> renderingTextureCubemapResourcesUpdate;
    std::vector<CMaterial*> materials;
    
    CFogComponent* fogComponent; 
};