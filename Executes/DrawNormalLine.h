#pragma once
#include "Execute.h"

class DrawNormalLine : public Execute
{
private:

	typedef VertexColor VertexType;
public:
	DrawNormalLine(ExecuteValues* values, D3DXVECTOR3 * startPos, D3DXVECTOR3 * endPos, UINT lineCount);
	~DrawNormalLine();

	// Execute을(를) 통해 상속됨
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();

private:
	Shader* shader;
	WorldBuffer * worldBuffer;

	ID3D11Buffer * vertexBuffer;

	UINT vertexCount, indexCount;

	VertexType * vertices;

	D3DXVECTOR3 * startPos;
	D3DXVECTOR3 * endPos;
	UINT lineCount;

};