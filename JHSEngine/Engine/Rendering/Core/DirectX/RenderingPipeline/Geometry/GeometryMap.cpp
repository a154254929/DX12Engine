#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"

FGeometryMap::FGeometryMap()
{
	geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::PreDraw(float deltaTime)
{
	descriptorHeap.PreDraw(deltaTime);
}

void FGeometryMap::Draw(float deltaTime)
{
	//渲染视口
	DrawViewport(deltaTime);

	//渲染模型
	DrawMesh(deltaTime);
}

void FGeometryMap::PostDraw(float deltaTime)
{

}

void FGeometryMap::UpdateCalculations(float deltaTime, const FViewportInfo viewportInfo)
{
	//XMINT3 cameraPos = XMINT3(viewportInfo.viewMatrix.m[3][0], viewportInfo.viewMatrix.m[3][1], viewportInfo.viewMatrix.m[3][2]);
	//XMINT3 cameraPos = XMINT3(5.0f, 5.0f, 5.0f);

	 //XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	 //XMVECTOR viewTarget = XMVectorZero();
	 //XMVECTOR viewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

	 //XMMATRIX viewLookAt = XMMatrixLookAtLH(pos, viewTarget, viewUp);
	 //XMStoreFloat4x4(const_cast<XMFLOAT4X4*>(&viewportInfo.viewMatrix), viewLookAt);
	 //XMMATRIX viewLookAt = XMLoadFloat4x4(&viewportInfo.viewMatrix);

	 //XMMATRIX artixProject = XMLoadFloat4x4(&viewportInfo.projectMatrix);
	 //XMMATRIX wvp = artixWorld * viewLookAt * artixProject;

	//XMMATRIX wrold = XMLoadFloat4x4(&worldMatrix);
	for (int i = 0; i < geometrys.size(); ++i)
	{
		for (size_t j = 0; j < geometrys[i].describeMeshRenderingData.size(); ++j)
		{
			FRenderingData& inRenderingData = geometrys[i].describeMeshRenderingData[j];
			{
				XMFLOAT3& position = inRenderingData.mesh->GetPosition();
				fvector_3d scale = inRenderingData.mesh->GetScale();

				XMFLOAT3 rightVector = inRenderingData.mesh->GetRightVector();
				XMFLOAT3 upVector = inRenderingData.mesh->GetUpVector();
				XMFLOAT3 forwardVector = inRenderingData.mesh->GetForwardVector();

				inRenderingData.worldMatrix = {
					rightVector.x * scale.x,	upVector.x,				forwardVector.x ,			0.f,
					rightVector.y,				upVector.y * scale.y,	forwardVector.y,			0.f,
					rightVector.z,				upVector.z ,			forwardVector.z * scale.z,	0.f,
					position.x,					position.y,				position.z,					1.f
				};
			}
			XMMATRIX artixWorld = XMLoadFloat4x4(&inRenderingData.worldMatrix);
			FObjectTransformation objectTransformation;
			XMStoreFloat4x4(&objectTransformation.world, XMMatrixTranspose(artixWorld));
			objectConstantBufferView.Update(j, &objectTransformation);
		}
	}

	//更新视口
	XMMATRIX projectMatrix = XMLoadFloat4x4(&viewportInfo.projectMatrix);
	XMMATRIX viewMatrix = XMLoadFloat4x4(&viewportInfo.viewMatrix);
	XMMATRIX viewProjMatrix = XMMatrixMultiply(viewMatrix, projectMatrix);

	FViewportTransformation viewportTransformation;
	XMStoreFloat4x4(&viewportTransformation.viewProjectionMatrix, XMMatrixTranspose(viewProjMatrix));
	viewportConstantBufferView.Update(0, &viewportTransformation);
}

void FGeometryMap::BuildMesh(GMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	FGeometry& geometry = geometrys[0];
	geometry.BuildMesh(inMesh, inMeshData);
}

void FGeometryMap::Build()
{
	//构建模型
	for (auto& tmp : geometrys)
	{
		tmp.second.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1是因为有摄像机
	descriptorHeap.Build(GetDrawObjectNumber() + 1);
}

void FGeometryMap::BuildObjectConstantBuffer()
{
	//创建常量缓冲区
	objectConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());

	CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//构建常量缓冲区
	objectConstantBufferView.BuildConstantBuffer(desHandle, GetDrawObjectNumber());
}

void FGeometryMap::BuildViewportConstantBuffer()
{
	//创建常量缓冲区
	viewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 1);

	CD3DX12_CPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//构建常量缓冲区
	viewportConstantBufferView.BuildConstantBuffer(desHandle, 1, GetDrawObjectNumber());
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return geometrys[0].GetDrawObjectNumber();
}

void FGeometryMap::DrawViewport(float deltaTime)
{
	UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

	desHandle.Offset(GetDrawObjectNumber(), descriptorOffset);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, desHandle);
}

void FGeometryMap::DrawMesh(float deltaTime)
{
	UINT descriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& tmp : geometrys)
	{
		D3D12_VERTEX_BUFFER_VIEW vbv = tmp.second.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW ibv = tmp.second.GetIndexBufferView();
		for (int j = 0; j < tmp.second.describeMeshRenderingData.size(); ++j)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE desHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
			FRenderingData& inRenderingData = tmp.second.describeMeshRenderingData[j];
			GetGraphicsCommandList()->IASetIndexBuffer(&ibv);
			//绑定渲染流水线是的输入槽,可以在输入装配阶段转入顶点数据
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//起始输入槽0-15
				1,
				&vbv
			);

			//定义要绘制哪种图元 点 线 面
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			desHandle.Offset(j, descriptorOffset);
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, desHandle);

			//真正绘制
			GetGraphicsCommandList()->DrawIndexedInstanced(
				inRenderingData.indexSize,//顶点索引数量
				1,//绘制数量
				inRenderingData.indexOffsetPosition,//顶点缓冲区第一个被绘制的索引
				inRenderingData.vertexOffsetPosition,//GPU从索引缓冲区读取的第一个索引位置
				0//在从顶点缓冲区中读取每个实例数据之前天道到每个索引的值
			);
		}
	}
}

bool FGeometry::bRenderingDataExistence(GMesh* inKey)
{
	for (auto& tmp : describeMeshRenderingData)
	{
		if (tmp.mesh == inKey)
			return true;
	}
	return false;
}

void FGeometry::BuildMesh(GMesh* inMesh, const FMeshRenderingData& inMeshData)
{
	if (!bRenderingDataExistence(inMesh))
	{
		describeMeshRenderingData.push_back(FRenderingData());
		FRenderingData& inRenderingData = describeMeshRenderingData[describeMeshRenderingData.size() - 1];

		//基础信息记录
		inRenderingData.mesh = inMesh;
		inRenderingData.indexSize = inMeshData.indexData.size();
		inRenderingData.vertexSize = inMeshData.vertexData.size();

		inRenderingData.indexOffsetPosition = meshRenderingData.indexData.size();
		inRenderingData.vertexOffsetPosition = meshRenderingData.vertexData.size();

		//索引的合并
		meshRenderingData.indexData.insert(
			meshRenderingData.indexData.end(),
			inMeshData.indexData.begin(),
			inMeshData.indexData.end()
		);
		//顶点的合并
		meshRenderingData.vertexData.insert(
			meshRenderingData.vertexData.end(),
			inMeshData.vertexData.begin(),
			inMeshData.vertexData.end()
		);
	}
}

void FGeometry::Build()
{
	//构建模型
	UINT vertexSizeInBytes = meshRenderingData.GetVertexSizeInBytes();
	UINT indexSizeInBytes = meshRenderingData.GetIndexSizeInBytes();

	ANALYSIS_HRESULT(D3DCreateBlob(
		vertexSizeInBytes,
		&cpuVertexBufferPtr
	));
	memcpy(
		cpuVertexBufferPtr->GetBufferPointer(),
		meshRenderingData.vertexData.data(),
		vertexSizeInBytes
	);

	ANALYSIS_HRESULT(D3DCreateBlob(
		indexSizeInBytes,
		&cpuIndexBufferPtr
	));
	memcpy(
		cpuIndexBufferPtr->GetBufferPointer(),
		meshRenderingData.indexData.data(),
		indexSizeInBytes
	);

	ConstructBuffer::FConstructBuffer constructBuffer;
	gpuVertexBufferPtr = constructBuffer.ConstructDefaultBuffer(
		vertexBufferTmpPtr,
		meshRenderingData.vertexData.data(),
		vertexSizeInBytes
	);
	gpuIndexBufferPtr = constructBuffer.ConstructDefaultBuffer(
		indexBufferTmpPtr,
		meshRenderingData.indexData.data(),
		indexSizeInBytes
	);
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = gpuVertexBufferPtr->GetGPUVirtualAddress();
	vbv.SizeInBytes = meshRenderingData.GetVertexSizeInBytes();
	vbv.StrideInBytes = sizeof(FVertex);
	return vbv;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = gpuIndexBufferPtr->GetGPUVirtualAddress();
	ibv.SizeInBytes = meshRenderingData.GetIndexSizeInBytes();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	return ibv;
}
