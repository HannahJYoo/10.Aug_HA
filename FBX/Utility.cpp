#include "stdafx.h"
#include "Utility.h"

D3DXMATRIX Fbx::Utility::Negative()	//fbx ist RH, also es wechselt LH
{
	D3DXMATRIX scaleX;
	D3DXMatrixScaling(&scaleX, -1, 1, 1);


	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, Math::ToRadian(180));

	return scaleX * rotation;
	
}

D3DXVECTOR2 Fbx::Utility::ToVector2(FbxVector2 & vec)
{
	return D3DXVECTOR2((float)vec.mData[0], (float)vec.mData[1]);
}

D3DXVECTOR3 Fbx::Utility::ToVector3(FbxVector4 & vec)
{
	return D3DXVECTOR3((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2]);
}

D3DXCOLOR Fbx::Utility::ToColor(FbxVector4 & vec)
{
	return D3DXCOLOR((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2], 1);
}

D3DXCOLOR Fbx::Utility::ToColor(FbxPropertyT<FbxDouble3>& vec, FbxPropertyT<FbxDouble>& factor)
{
	FbxDouble3 color = vec;

	D3DXCOLOR result;
	result.r = (float)color.mData[0];
	result.g = (float)color.mData[1];
	result.b = (float)color.mData[2];
	result.a = (float)factor;

	return result;
}

D3DXMATRIX Fbx::Utility::ToMatrix(FbxAMatrix & matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	D3DXMATRIX origin = D3DXMATRIX
	(
		(float)r1.mData[0], (float)r1.mData[1], (float)r1.mData[2], (float)r1.mData[3],
		(float)r2.mData[0], (float)r2.mData[1], (float)r2.mData[2], (float)r2.mData[3],
		(float)r3.mData[0], (float)r3.mData[1], (float)r3.mData[2], (float)r3.mData[3],
		(float)r4.mData[0], (float)r4.mData[1], (float)r4.mData[2], (float)r4.mData[3]
	);

	return Negative() * origin * Negative();
}

string Fbx::Utility::GetTextureFile(FbxProperty & prop)
{
	if (prop.IsValid() == true) {
		if (prop.GetSrcObjectCount() > 0) {
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>();

			if (texture != NULL)
				return string(texture->GetFileName());
		}
	}

	return "";
}

string Fbx::Utility::GetMaterialName(FbxMesh * mesh, int polygonIndex, int cpIndex)
{	//Get Material Name fuer Vergleichen Meshpart.

	FbxNode* node = mesh->GetNode();	//get Node from mesh.
	if (node == NULL) return "";

	FbxLayerElementMaterial* material = mesh->GetLayer(0)->GetMaterials();	
	//nutzen Layer immer nur Null im GameProgramming.
	if (material == NULL) return "";

	FbxLayerElement::EMappingMode mappingMode = material->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = material->GetReferenceMode();

	int mappingIndex = -1;

	switch (mappingMode) {	
	case FbxLayerElement::eAllSame: mappingIndex = 0; break;	//meshIndex = materialIndex
	case FbxLayerElement::eByPolygon: mappingIndex = polygonIndex; break;	//Triangle기준으로 material 연결
	case FbxLayerElement::eByControlPoint: mappingIndex = cpIndex; break;	//fbx 자체에서 처리 ControlPoint기준으로 연결
	case FbxLayerElement::eByPolygonVertex: mappingIndex = polygonIndex * 3; break;	//정점기준으로 바뀜 polygon은 삼각형으로 쓸 것 이기 때문에 *3해줌
	default: assert(false); break;
	}

	//위 번호로 찾음
	FbxSurfaceMaterial* findMaterial = NULL;
	if (refMode == FbxLayerElement::eDirect) {	//mappingIndex가 바로 참조 가능할 때.
		if (mappingIndex < node->GetMaterialCount())
			findMaterial = node->GetMaterial(mappingIndex);
	}

	else if (refMode == FbxLayerElement::eIndexToDirect) {	//mappingIndex가 다른 참조번호를 가르켜 참조할 때.
		FbxLayerElementArrayTemplate<int>& indexArr = material->GetIndexArray();

		if (mappingIndex < indexArr.GetCount()) {	
			int tempIndex = indexArr.GetAt(mappingIndex);
			
			if (tempIndex < node->GetMaterialCount())
				findMaterial = node->GetMaterial(tempIndex);
		}//if(mappinIndex)
	}//else if(refMode)

	if (findMaterial == NULL)
		return "";

	return findMaterial->GetName();
}

D3DXVECTOR2 Fbx::Utility::GetUv(FbxMesh * mesh, int cpIndex, int uvIndex) //Get UV Index number
{

	D3DXVECTOR2 result = D3DXVECTOR2(0, 0);

	FbxLayerElementUV* uv = mesh->GetLayer(0)->GetUVs();
	if (uv == NULL) return result;

	FbxLayerElement::EMappingMode mappingMode = uv->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = uv->GetReferenceMode();

	switch (mappingMode) {
	case FbxLayerElement::eByControlPoint: 
	{
		if (refMode == FbxLayerElement::eDirect) {
			result.x = (float)uv->GetDirectArray().GetAt(cpIndex).mData[0];	//uv는 배열로 사용.
			result.y = (float)uv->GetDirectArray().GetAt(cpIndex).mData[1];
		}
		else if (refMode == FbxLayerElement::eIndexToDirect) {
			int index = uv->GetIndexArray().GetAt(cpIndex);

			result.x = (float)uv->GetDirectArray().GetAt(index).mData[0];
			result.y = (float)uv->GetDirectArray().GetAt(index).mData[1];

		}
	}
	 break;

	case FbxLayerElement::eByPolygonVertex:
	{
		result.x = (float)uv->GetDirectArray().GetAt(uvIndex).mData[0];
		result.y = (float)uv->GetDirectArray().GetAt(uvIndex).mData[1];
	}
	break;
										  
	}
	result.y = 1.0f - result.y;	//fbx = bottom->top also reverse.

	return result;
}
