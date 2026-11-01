#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../RenderBuffer/NormalBuffer.h"

struct FScreenSpaceAmbientOcclusion
    : public IDirectXDeviceInterface_Struct
{
public:
    virtual void Init(
        FGeometryMap* inGeometryMap,
        FDirectXPipelineState* inDirectXPipelineState,
        FRenderLayerManager* inRenderLayerManager
    );
    
    virtual void Init(int inWidth, int inHeight);
    
    virtual void Draw(float deltaTime);
    virtual void UpdateCalculations(float deltaTime, const FViewportInfo& inViewportInfo);
    
    virtual void BuildDescriptors();
    
protected:
    FNormalBuffer normalBuffer;
    
};