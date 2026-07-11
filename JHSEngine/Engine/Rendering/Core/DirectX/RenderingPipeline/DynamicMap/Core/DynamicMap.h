#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../RenderTarget/Core/RenderTarget.h"

class FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManager;
class FViewportInfo;

class FDynamicMap : public IDirectXDeviceInterface
{
public:
    FDynamicMap();
    
public:
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
        );
    
    virtual void UpdateCalculations(
        float deltaTime,
        const FViewportInfo& inViewportInfo
    ); 
    
    virtual void PreDraw(float deltaTime);
    
    virtual void Draw(float deltaTime);
    
public:
    template<class T>
    void CreateRenderTarget();

protected:
    std::unique_ptr<FRenderTarget> renderTarget;
    
    FGeometryMap* geometryMap;                          //几何Map
    FDirectXPipelineState* directXPipelineState;        //管线对象，用于绑定
    
    FRenderLayerManager* renderLayerManager;                          //渲染层级
    
    UINT width;
    UINT height;
};

template <class T>
void FDynamicMap::CreateRenderTarget()
{
    if (!renderTarget)
    {
        renderTarget = std::make_unique<T>();
    }
}
