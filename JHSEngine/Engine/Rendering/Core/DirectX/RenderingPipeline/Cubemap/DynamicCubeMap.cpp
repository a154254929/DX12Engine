#include "DynamicCubeMap.h"
#include "../Geometry/GeometryMap.h"
#include "../PipelineState/DirectXPipelineState.h"
#include "../RenderTarget/CubeMapRenderTarget.h"
#include "../../../../../Core/Viewport/ClientViewport.h"


FDynamicCubeMap::FDynamicCubeMap()
    : geometryMap(NULL)
    , directXPipelineState(NULL)
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
    geometryMap = inGeometryMap;
    directXPipelineState = inDirectXPipelineState;
}

void FDynamicCubeMap::BuildViewPort(const fvector_3d& inPosition)
{
    struct FTmpViewportCapture
    {
        fvector_3d targetPosition[6];
        fvector_3d up[6] = {
            fvector_3d(0, 1, 0),
            fvector_3d(0, 1, 0),
            fvector_3d(0, 0, -1),
            fvector_3d(0, 0, 1),
            fvector_3d(0, 1, 0),
        };
    };
    
    FTmpViewportCapture tmpCapture;
    
    tmpCapture.targetPosition[0] = fvector_3d(inPosition.x + 1.0f, inPosition.y, inPosition.z);
    tmpCapture.targetPosition[1] = fvector_3d(inPosition.x - 1.0f, inPosition.y, inPosition.z);
    tmpCapture.targetPosition[2] = fvector_3d(inPosition.x, inPosition.y + 1.0f, inPosition.z);
    tmpCapture.targetPosition[3] = fvector_3d(inPosition.x, inPosition.y - 1.0f, inPosition.z);
    tmpCapture.targetPosition[4] = fvector_3d(inPosition.x, inPosition.y, inPosition.z + 1.0f);
    tmpCapture.targetPosition[5] = fvector_3d(inPosition.x, inPosition.y, inPosition.z - 1.0f);
    
    for (int i = 0; i < 6; i++)
    {
        GClientViewport* viewport = CreateObject<GClientViewport>(new GClientViewport());
        viewports.push_back(viewport);
        
        viewport->LookAt(
            inPosition,
            tmpCapture.targetPosition[i],
            tmpCapture.up[i]
        );
        viewport->SetFrustum(90.f, 1.f, 0.1f, 1000.f);
        
        viewport->BuildViewMatrix(0.016f);
    }
}

void FDynamicCubeMap::BuildDepthStencil()
{
}
