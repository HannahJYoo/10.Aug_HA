#pragma once
#include "GameRender.h"

class GameModel : public GameRender
{
public:
	GameModel(wstring matFolder, wstring matFile, wstring meshFolder, wstring meshFile);
	virtual ~GameModel();

	void Velocity(D3DXVECTOR3& vec);
	D3DXVECTOR3 Velocity();

	virtual void Update();
	virtual void Render();

	void Rotate(D3DXVECTOR2 amount);

protected:
	void CalcPosition();

protected:
	Model* model;
	Shader* shader;

	D3DXVECTOR3 velocity;

	D3DXMATRIX matRotation;
	vector<D3DXMATRIX> boneTransforms;
	
//	////////////////////
//private:
//	ModelBone* turretBone;

private:
	class RenderBuffer : public ShaderBuffer
	{
	public:
		RenderBuffer() :ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Index = 0;
		}

		struct Struct
		{
			int Index;
			float Padding[3];

		}Data;
	};

	RenderBuffer* renderBuffer;


	////////////////////////////////////////////////
public:
	void Center(D3DXVECTOR3& val) { val = center; }
	void BoundSize(D3DXVECTOR3& val) { val = boundSize; }
	void Radius(float& val) { val = radius; }

	void GetAABB(std::vector<D3DXVECTOR3>& aabbBox);

	D3DXVECTOR3 MinVertice() { return vecMin; }
	D3DXVECTOR3 MaxVertice() { return vecMax; }

	
protected:
	std::vector<D3DXVECTOR3> boundSpace;
	D3DXVECTOR3 center;
	D3DXVECTOR3 boundSize;
	D3DXVECTOR3 vecMin, vecMax;
	float radius;

	void SetBoundSpace();
	///////////////////////////////////////////////
};