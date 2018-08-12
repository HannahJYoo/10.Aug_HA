#pragma once
#include "Execute.h"

class DrawNormalLine;
class ExeGrid : public Execute
{
private:

	typedef VertexTextureNormal VertexType;

public:
	ExeGrid(ExecuteValues* values);
	~ExeGrid();

	// Execute을(를) 통해 상속됨
	 void Update() ;
	 void PreRender() ;
	 void Render() ;
	 void PostRender() ;
	 void ResizeScreen() ;


private:
	Material* material;
	WorldBuffer* worldBuffer;

	UINT width, height;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	//Set normal Data
	D3DXVECTOR3 * startPos;
	D3DXVECTOR3 * endPos;
	
	VertexType* vertices;
	UINT* indices;


	Texture* texture[2];

	int fillModeNumber;
	ID3D11RasterizerState* fillMode[2];

	DrawNormalLine* draw;
private:
	void CreateNormal();


	

};