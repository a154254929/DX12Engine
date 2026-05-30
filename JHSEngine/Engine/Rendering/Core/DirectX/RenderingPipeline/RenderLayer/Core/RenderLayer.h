#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../../Shader/Core/Shader.h"
#include "../../Geometry/RenderingData.h"

class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
public:
    FRenderLayer();
    
protected:
    UINT renderPriority;
};