#include "DynamicShadowMap.h"

#include "../../RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
    : Super()
{
    CreateRenderTarget<FShadowMapRenderTarget>();
}
