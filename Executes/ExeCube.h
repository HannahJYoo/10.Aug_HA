#pragma once
#include "Execute.h"

class ExeCube : public Execute
{
private:
	class ColorBuffer;
	class UvBuffer;

public:
	ExeCube(ExecuteValues* values);
	~ExeCube();

	// Execute¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	 void Update() ;
	 void PreRender() ;
	 void Render() ;
	 void PostRender() ;
	 void ResizeScreen() ;

	 void SetSRV(wstring str);

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
	UvBuffer* uvBuffer;

	ID3D11ShaderResourceView* srv[12];	//texture
		
	float uvoffset[6];

	ID3D11SamplerState* state[4];

	D3DXVECTOR3 location;
	D3DXVECTOR3 rotation;
	D3DXVECTOR2 scale;

	D3DXMATRIX world;

	ID3D11RasterizerState* rasterizer[2];

	function<void(wstring)> loadSrv;
	int textureList;


private:
	class ColorBuffer : public ShaderBuffer {
	public:
		ColorBuffer()
			:ShaderBuffer(&Data,sizeof(Data))
		{
			Data.Color = D3DXCOLOR(1, 1,1,1);
			
		}
		~ColorBuffer() 
		{
		
		}
		
		struct Struct {
			D3DXCOLOR Color;
			
		} Data;
	};

	class UvBuffer : public ShaderBuffer {
	public:
		UvBuffer()
			:ShaderBuffer(&Data, sizeof(Data))
		{
			Data.uv_offset = 0.5f;
		}
		~UvBuffer()
		{

		}
		
		struct Struct {
			float uv_offset;
			long long temp;	//padding bit (size æ»∏¬√Á¡÷∏È ≈Õ¡¸)
		} Data;
	};




};