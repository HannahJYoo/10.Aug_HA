#include "stdafx.h"
#include "ExeNormal.h"

ExeNormal::ExeNormal(ExecuteValues * values)
	:Execute(values)
{
	Texture* heightMap = new Texture(Contents + L"HeightMaps/HeightMap.png");
	vector<D3DXCOLOR> heightColor;
	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heightColor);

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	shader = new Shader(Shaders + L"008_Diffuse.hlsl", "VS", "PS_Diffuse");
	normalShader = new Shader(Shaders + L"003_Color.hlsl");

	worldBuffer = new WorldBuffer();
	colorBuffer = new ColorBuffer;

	//load Texture
	texture[0] = new Texture(Contents + L"Textures/dirt.png");
	texture[1] = new Texture(Contents + L"Textures/Wall.png");
	texture[2] = new Texture(Contents + L"HeightMaps/AlphaMap.png");

	//Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexType[vertexCount];

		for (UINT z = 0; z <= height; z++) {
			for (UINT x = 0; x <= width; x++) {
				UINT index = (width + 1) * z + x;
				vertices[index].Position.x = (float)x - (width / 2);
				vertices[index].Position.y = (float)((heightColor[index].r * 256.0f) / 7.5f) - 100;
				vertices[index].Position.z = (float)z - (height / 2);

				vertices[index].Uv.x = (float)x / (width);
				vertices[index].Uv.y = (float)z / (height);

			}
		}//for(z)
	}

	//Create IndexData
	{
		indexCount = width * height * 6;
		indices = new UINT[indexCount];

		UINT index = 0;
		for (UINT z = 0; z < height; z++) {
			for (UINT x = 0; x < width; x++) {
				indices[index + 0] = (width + 1) * (z + 0) + x;
				indices[index + 1] = (width + 1) * (z + 1) + x;
				indices[index + 2] = (width + 1) * (z + 0) + x + 1;

				indices[index + 3] = (width + 1) * (z + 0) + x + 1;
				indices[index + 4] = (width + 1) * (z + 1) + x;
				indices[index + 5] = (width + 1) * (z + 1) + x + 1;

				index += 6;
			}
		}
	}


	CreateNormal();

	//CreateNormalData
		//Create normal VertexData
	{
		normalVertexCount = (width + 1) * (height + 1) * 2;
		normalVertices = new VertexColor[normalVertexCount];

		UINT index = 0;
		for (index = 0; index < normalVertexCount / 2; index++)
		{
			normalVertices[index * 2].Position = vertices[index].Position;
			normalVertices[index * 2 + 1].Position = vertices[index].Position + vertices[index].Normal;

			normalVertices[index * 2].Color = D3DXCOLOR(0, 0, 1, 1);
			normalVertices[index * 2 + 1].Color = D3DXCOLOR(0, 0, 1, 1);
		}
	}

	//Create normal IndexData
	{
		normalIndexCount = normalVertexCount;
		normalIndices = new UINT[normalIndexCount];
		UINT index = 0;
		for (index = 0; index < normalIndexCount; index++)
			normalIndices[index] = index;
	}



	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexType) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Buffer about Normal
		//create normal VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexColor) * normalVertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = normalVertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &normalVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//IndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * normalIndexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = normalIndices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &normalIndexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Set fillMode
	{
		fillModeNumber = 0;
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);

		States::CreateRasterizer(&desc, &fillMode[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &fillMode[1]);
	}

}

ExeNormal::~ExeNormal()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(normalIndexBuffer);
	SAFE_RELEASE(normalVertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(normalIndices);
	SAFE_DELETE_ARRAY(normalVertices);

	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeNormal::Update()
{

}

void ExeNormal::PreRender()
{

}

void ExeNormal::Render()
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	texture[0]->SetShaderResource(0);
	texture[1]->SetShaderResource(1);
	texture[2]->SetShaderResource(2);


	D3D::GetDC()->RSSetState(fillMode[fillModeNumber]);
	//
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	worldBuffer->SetMatrix(world);
	//
	colorBuffer->SetPSBuffer(0);
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	//실제 그릴 코드
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);

	if (fillModeNumber == 1) {

		UINT norStride = sizeof(VertexColor);
		//D3DXMATRIX world;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &normalVertexBuffer, &norStride, &offset);
		D3D::GetDC()->IASetIndexBuffer(normalIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		D3DXMatrixIdentity(&world);

		worldBuffer->SetMatrix(world);
		worldBuffer->SetVSBuffer(1);
		normalShader->Render();

		D3D::GetDC()->DrawIndexed(normalIndexCount, 0, 0);

	}
}

void ExeNormal::PostRender()
{
	ImGui::SliderInt("Wireframe", &fillModeNumber, 0, 1);
	ImGui::SliderFloat3("Direction", (float*)colorBuffer->Data.Light, -1, 1);
}

void ExeNormal::ResizeScreen()
{
}

void ExeNormal::CreateNormal()
{
	for (UINT i = 0; i < (indexCount / 3); i++) {
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType v0 = vertices[index0];
		VertexType v1 = vertices[index1];
		VertexType v2 = vertices[index2];

		D3DXVECTOR3 d1 = v1.Position - v0.Position;
		D3DXVECTOR3 d2 = v2.Position - v0.Position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);

}
