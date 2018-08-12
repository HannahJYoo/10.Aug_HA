#pragma once
#include "Execute.h"

class ExeTexture : public Execute
{
private:
	class ColorBuffer;

public:
	ExeTexture(ExecuteValues* values);
	~ExeTexture();

	// Execute을(를) 통해 상속됨
	 void Update() ;
	 void PreRender() ;
	 void Render() ;
	 void PostRender() ;
	 void ResizeScreen() ;

private:
	int number;
	int number2;

	Shader * shader;
	WorldBuffer* worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	VertexTexture* vertices;
	UINT* indices;

	ColorBuffer* colorBuffer;

	ID3D11ShaderResourceView* srv;	//texture
	ID3D11ShaderResourceView* srv2;	//texture

	ID3D11SamplerState* state[4];

	D3DXVECTOR2 location;
	D3DXVECTOR3 rotation;
	D3DXVECTOR2 scale;

	D3DXMATRIX world;

	ID3D11RasterizerState* rasterizer[2];


private:
	class ColorBuffer : public ShaderBuffer {
	public:
		ColorBuffer()
			:ShaderBuffer(&Data,sizeof(Data))
		{
			Data.Color = D3DXCOLOR(1, 1, 1, 1);
		}
		~ColorBuffer() 
		{
		
		}
		struct Struct {
			D3DXCOLOR Color;
		} Data;
	};

};