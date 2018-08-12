#pragma once
#include "Execute.h"

class ExeNormal : public Execute
{
private:
	class ColorBuffer;

	typedef VertexTextureNormal VertexType;

public:
	ExeNormal(ExecuteValues* values);
	~ExeNormal();

	// Execute을(를) 통해 상속됨
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();


private:
	Shader * shader;
	WorldBuffer* worldBuffer;

	UINT width, height;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	//Set normal Data
	Shader * normalShader;
	ID3D11Buffer* normalVertexBuffer;
	ID3D11Buffer* normalIndexBuffer;
	UINT normalVertexCount, normalIndexCount;
	VertexColor* normalVertices;
	UINT* normalIndices;
	//

	VertexType* vertices;
	UINT* indices;

	ColorBuffer* colorBuffer;

	Texture* texture[3];

	int fillModeNumber;
	ID3D11RasterizerState* fillMode[2];
private:
	void CreateNormal();

private:
	class ColorBuffer : public ShaderBuffer {
	public:
		ColorBuffer()
			:ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Light = D3DXVECTOR3(-1, -1, -1);
		}
		~ColorBuffer()
		{

		}
		struct Struct {
			D3DXVECTOR3 Light;
			float Padding;
		} Data;
	};



};