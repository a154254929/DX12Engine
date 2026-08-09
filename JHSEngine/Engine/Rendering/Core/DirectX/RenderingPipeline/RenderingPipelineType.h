#pragma once

enum EPipelineState
{
    Opaque = 0,
    AlphaTest = 1,
    Transparent = 2,
    PostProcess = 3,
    BackGround = 4,
    OpaqueReflector = 5,
    GrayModel = 6,
    WireFrame = 7,
    OrthogonalShadowShadow = 8,
    PerspectiveShadowShadow = 9,
    VientianeShadowShadow,
};

enum ERenderingConditions
{
    RC_None = 0,
    RC_Shadow,
    
};