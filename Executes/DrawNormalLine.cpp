#include "stdafx.h"
#include "DrawNormalLine.h"

DrawNormalLine::DrawNormalLine(ExecuteValues* values, D3DXVECTOR3 * startPos, D3DXVECTOR3 * endPos, UINT lineCount)
	:Execute(values),
	vertexCount(lineCount * 2)
{
	shader = new Shader(Shaders + L"008_Diffuse.hlsl");
	worldBuffer = new WorldBuffer();

	// Create VertexData
	{
		vertices = new VertexType[vertexCount];

		for (UINT i = 0; i < vertexCount; i++) {
			if (i % 2 == 0) 
				vertices[i].Position = startPos[i / 2];
			
			else 
				vertices[i].Position = endPos[i / 2];
			
			vertices[i].Color = D3DXCOLOR(0, 0, 1, 1);
		}
	}

	// CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexType) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA  data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}


}

DrawNormalLine::~DrawNormalLine()
{
	
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void DrawNormalLine::Update()
{

}

void DrawNormalLine::PreRender()
{
}

void DrawNormalLine::Render()
{
	UINT stride = sizeof(VertexType); 
	UINT offset = 0;

	
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	// 실제 그릴 코드
	D3D::GetDC()->Draw(vertexCount, 0);
}

void DrawNormalLine::PostRender()
{

}

void DrawNormalLine::ResizeScreen()
{
}


