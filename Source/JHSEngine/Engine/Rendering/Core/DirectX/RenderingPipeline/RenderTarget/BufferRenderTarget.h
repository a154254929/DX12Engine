#pragma once
#include "Core/RenderTarget.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FRenderTargetDelegate, void, ComPtr<ID3D12Resource>&);

class FBufferRenderTarget : public FRenderTarget
{
  typedef FRenderTarget Super;
  
public:
  FRenderTargetDelegate renderTargetDelegate;
  
  virtual void Init(UINT inWidth, UINT inHeight, DXGI_FORMAT inFormat);
  
  CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPURenderTargetView() { return renderTargetViewCPU; };
  
public:
  FBufferRenderTarget();
  
protected:
  virtual void BuildRenderTargetMap();
  virtual void BuildSRVDescriptors();
  virtual void BuildRTVDescriptors();
  
protected:
  CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetViewCPU;
};