#include "Texture2D.h"
#include "Engine/Engine.h"
#include "Engine/Base/CommandDirectX.h"
#include "Engine/Manager/ShaderManager.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Log.h"

#include "Engine/Base/GraphicsPipeline/GraphicsPipeline.h"


void Texture2D::Finalize()
{
	graphicsPipelineState->Release();
	rootSignature->Release();
	indexResource->Release();
	vertexShader->Release();
	vertexResource->Release();
	SRVHeap->Release();
	pixelShader->Release();
	resource[0]->Release();
}

void Texture2D::Texture(const std::string& filePath, const std::string& vsFileName, const std::string& psFileName)
{
	blend = BlendMode::Normal;

	CreateDescriptor(filePath);

	vertexShader = GraphicsPipeline::GetInstance()->CreateVSShader(vsFileName);
	//	ピクセルシェーダーのコンパイルがなぜかできないため、緊急措置を行っている
	pixelShader = ShaderManager::GetInstance()->CompileShader(ConvertString(psFileName), L"ps_6_0");
	//pixelShader = GraphicsPipeline::GetInstance()->CreatePSShader(psFileName);
	GraphicsPipeline::GetInstance()->pixelShader = pixelShader;

	CreateVertexResource();


	D3D12_DESCRIPTOR_RANGE range[1] = {};
	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 1;	//	必要な数
	range[0].RegisterSpace = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].Descriptor.ShaderRegister = D3D12_SHADER_VISIBILITY_ALL;
	rootParameter[0].DescriptorTable.pDescriptorRanges = range;
	rootParameter[0].DescriptorTable.NumDescriptorRanges = _countof(range);

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[1].Descriptor.ShaderRegister = 0;

	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].Descriptor.ShaderRegister = 1;

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[3].Descriptor.ShaderRegister = 2;


	rootSignature = GraphicsPipeline::GetInstance()->CreateRootSignature(rootParameter, 4);
	graphicsPipelineState = GraphicsPipeline::GetInstance()->CreateGraphicsPipeline();
}

void Texture2D::CreateDescriptor(const std::string& filePath)
{
	DirectX::ScratchImage mipImages = TextureManager::LoadTexture(filePath);
	const DirectX::TexMetadata& metaData = mipImages.GetMetadata();
	resource[0] = Engine::CreateTextureResource(Engine::GetDevice(), metaData);
	TextureManager::UploadTextureData(resource[0], mipImages);

	//	画像サイズの代入
	textureWidth = static_cast<uint32_t>(metaData.width);
	textureHeight = static_cast<uint32_t>(metaData.height);

	//	デスクリプタヒープを生成
	SRVHeap = CreateDescriptorHeap(Engine::GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 10, true);

	//	設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = metaData.format;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//	
	Engine::GetDevice()->CreateShaderResourceView(resource[0], &srvDesc, SRVHeap->GetCPUDescriptorHandleForHeapStart());
}

void Texture2D::CreateVertexResource()
{
	//	頂点データ
	VertexData vertex[4] = {
		{{-0.5f,0.5f,0.1f,1.0f},{0.0f,0.0f}},
		{{0.5f,0.5f,0.1f,1.0f},{1.0f,0.0f}},
		{{0.5f,-0.5f,0.1f,1.0f},{1.0f,1.0f}},
		{{-0.5f,-0.5f,0.1f,1.0f},{0.0f,1.0f}},
	};
	vertexResource = Engine::CreateBufferResource(Engine::GetDevice(), sizeof(vertex));

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(vertex);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//	
	VertexData* mapData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mapData));
	for (auto i = 0; i < _countof(vertex); i++)
	{
		mapData[i] = vertex[i];
	}
	//	重要
	vertexResource->Unmap(0, nullptr);

	// index情報を作る
	// 1.indexの情報を送るためにmap用のuint16_t型の配列を作る(中身は頂点の組み合わせ、要素番号)
	uint16_t indices[6] = { 0,1,3,1,2,3 };
	// 2.Resourceを生成
	indexResource = Engine::CreateBufferResource(Engine::GetDevice(), sizeof(indices));
	// 3.indexBufferViewを生成
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView.SizeInBytes = sizeof(indices);

	// 4.GPUに送るためのMap用のuint16_tポインタを生成
	uint16_t* indexData = nullptr;

	// 5.resoureceのMap関数を呼んで関連付けさせる
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// 6.4で作ったものに3のものをコピー
	for (auto i = 0; i < _countof(indices); i++)
	{
		indexData[i] = indices[i];
	}

	// 7.Unmapする
	indexResource->Unmap(0, nullptr);
}

void Texture2D::Draw(Vector2 pos, Vector2 scale, float rotate, Matrix4x4 viewProjectionMat, uint32_t color)
{	
	/*ImGui::Begin("a");
	ImGui::DragFloat2("%0.2f", &cBuffer->pibot.x, 1.0f);
	ImGui::End();*/
	//	色の変更
	/**cColor = ChangeColor(color);
	*cMat = MakeAffineMatrix(
		{ scale.x * static_cast<float>(textureWidth),scale.y * static_cast<float>(textureHeight),1.0f },
		{ 0.0f,0.0f,rotate },
		{ pos.x,pos.y,0.5f }
	) * viewProjectionMat;

	Engine::GetList()->SetGraphicsRootSignature(rootSignature);
	Engine::GetList()->SetPipelineState(graphicsPipelineState);*/
	// インデックスを使わずに四角形以上を書くときは
	// 個々の設定はD3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	// インデックスを使うときは D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// ここにIndexBufferViewを設定
	Engine::GetList()->IASetIndexBuffer(&indexBufferView);

	Engine::GetList()->SetDescriptorHeaps(1, &SRVHeap);
	Engine::GetList()->SetGraphicsRootDescriptorTable(0, SRVHeap->GetGPUDescriptorHandleForHeapStart());
	Engine::GetList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Texture2D::TextureDraw(WorldTransform& worldTransform, const Matrix4x4& viewProjectionMat, uint32_t color, Texture2D* texture)
{
	*worldTransform.cMat = MakeAffineMatrix(
		{ worldTransform.scale_.x * static_cast<float>(texture->textureWidth),worldTransform.scale_.y * static_cast<float>(texture->textureHeight),1.0f },
		{ 0.0f,0.0f,worldTransform.rotation_.z },
		{ worldTransform.translation_.x,worldTransform.translation_.y,0.5f }
	) * viewProjectionMat;
	*worldTransform.cColor = ChangeColor(color);

	Engine::GetList()->SetGraphicsRootSignature(texture->rootSignature.Get());
	Engine::GetList()->SetPipelineState(texture->graphicsPipelineState.Get());
	// インデックスを使わずに四角形以上を書くときは
	// 個々の設定はD3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	// インデックスを使うときは D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	Engine::GetList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetList()->IASetVertexBuffers(0, 1, &texture->vertexBufferView);
	// ここにIndexBufferViewを設定
	Engine::GetList()->IASetIndexBuffer(&texture->indexBufferView);

	Engine::GetList()->SetDescriptorHeaps(1, &texture->SRVHeap);
	Engine::GetList()->SetGraphicsRootDescriptorTable(0, texture->SRVHeap->GetGPUDescriptorHandleForHeapStart());

	Engine::GetList()->SetGraphicsRootConstantBufferView(1, worldTransform.cMat.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(2, worldTransform.cColor.GetGPUVirtualAddress());
	Engine::GetList()->SetGraphicsRootConstantBufferView(3, worldTransform.cMono.GetGPUVirtualAddress());

	Engine::GetList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Texture2D::SetBlend(BlendMode blend_)
{
	blend = blend_;
	//CreateGraphicsPipeline();
}

