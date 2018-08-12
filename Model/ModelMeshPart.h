#pragma once
#include "./Interfaces/ICloneable.h"

class ModelMeshPart :public ICloneable
{
public:
	friend class Model;
	friend class Models;
	friend class ModelMesh;

private:
	ModelMeshPart();
	~ModelMeshPart();

	void Binding();
	void Render();

	void Clone(void** clone);
public:
	vector<ModelVertexType> Vertices() { return vertices; }

private:
	Material* material;
	wstring materialName;

	class ModelMesh* parent;

	vector<ModelVertexType> vertices;
	vector<UINT> indices;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};