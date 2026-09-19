#pragma once
#include "../Engine.h"

class FViewport
{
public:
    FViewport();

    void ViewportInit();
    
public:
    void ResetViewport(UINT inWidth, UINT inHeight);
    void ResetScissorRect(UINT inWidth, UINT inHeight);

public:
    //和屏幕的视口有关
    D3D12_VIEWPORT viewportInfo;
    D3D12_RECT viewportRect;

    //定义摄像机位置,UP, Right, Look
    XMFLOAT4X4 viewMatrix;
    //远近裁剪面 FOV
    XMFLOAT4X4 projectMatrix;
};