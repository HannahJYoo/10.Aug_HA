#include "stdafx.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "../Utilities\Xml.h"
#include "../Utilities\BinaryFile.h"



void Model::ReadMaterial(wstring folder, wstring file)
{
	/*Xml::XMLDocument* document = new Xml::XMLDocument();

	wstring tempFile = folder + file;

	Xml::XMLError error = document->LoadFile(String::ToString(tempFile).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();

	do
	{
		Xml::XMLElement* node = NULL;

		Material* material = new Material();
		
		node = matNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));

		node = node->NextSiblingElement();

		D3DXCOLOR dxColor;
		Xml::XMLElement* color = node->FirstChildElement();
		dxColor.r = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.g = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.b = color->FloatText();

		color = color->NextSiblingElement();
		dxColor.a = color->FloatText();
		material->SetDiffuse(dxColor);

		node = node->NextSiblingElement();
		wstring diffuseTexture = String::ToWString(node->GetText());
		material->SetDiffuseMap(folder + diffuseTexture);

		materials.push_back(material);
		matNode = matNode->NextSiblingElement();
	} while (matNode != NULL);*/

	Models::LoadMaterial(folder, file, &materials);

}
void Model::ReadMesh(wstring folder, wstring file)
{
	//BinaryReader* r = new BinaryReader();
	//r->Open(folder + file);

	//UINT count = 0;
	//count = r->UInt();

	//for (UINT i = 0; i < count; i++) {
	//	ModelBone* bone = new ModelBone();

	//	bone->index = r->Int();
	//	bone->name = String::ToWString(r->String());
	//	bone->parentIndex = r->Int();

	//	bone->local = r->Matrix();
	//	bone->global = r->Matrix();

	//	bones.push_back(bone);
	//}

	//count = r->UInt();
	//for (UINT i = 0; i < count; i++) {
	//	ModelMesh* mesh = new ModelMesh();
	//	mesh->name = String::ToWString(r->String());
	//	mesh->parentBoneIndex = r->Int();

	//	UINT partCount = r->UInt();
	//	for (UINT k = 0; k < partCount; k++) {
	//		ModelMeshPart* meshPart = new ModelMeshPart();
	//		meshPart->parent = mesh;

	//		meshPart->materialName = String::ToWString(r->String());
	//		//VertexData
	//		{
	//			UINT count = r->UInt();
	//			meshPart->vertices.assign(count, ModelVertexType());	//assign으로 미리 공간 확보후 불러옴.

	//			void* ptr = (void *)&(meshPart->vertices[0]);
	//			r->Byte(&ptr, sizeof(ModelVertexType) * count);
	//		}

	//		//IndexData
	//		{
	//			UINT count = r->UInt();
	//			meshPart->indices.assign(count, UINT());

	//			void* ptr = (void *)&(meshPart->indices[0]);
	//			r->Byte(&ptr, sizeof(UINT) * count);
	//		}

	//		mesh->meshParts.push_back(meshPart);
	//	}//for(k)
	//	meshes.push_back(mesh);
	//}//for(i)

	//r->Close();
	//SAFE_DELETE(r);

	Models::LoadMesh(folder, file, &bones, &meshes);
	BindingBone();
	BindingMesh();
}



void Model::BindingBone()
{
	this->root = bones[0];
	for (ModelBone* bone : bones) {
		if (bone->parentIndex > -1) {
			bone->parent = bones[bone->parentIndex];
			bone->parent->childs.push_back(bone);
		}
		else {
			bone->parent = NULL;
		}
	}
}

void Model::BindingMesh()
{
	for (ModelMesh* mesh : meshes) {
		for (ModelBone* bone : bones) {
			if (mesh->parentBoneIndex == bone->index) {
				mesh->parentBone = bone;
				break;
			}
		}//for(bone)

		for (ModelMeshPart* part : mesh->meshParts) {
			for (Material* material : materials) {
				if (material->Name() == part->materialName) {
					part->material = material;
					break;
				}
			}
		}//for(part)

		mesh->Binding();

	}//for(mesh)
}

//Models Function

map<wstring, vector<Material *>> Models::materialMap;


void Models::LoadMaterial(wstring matFolder, wstring matFile, vector<Material*>* materials)
{
	if (materialMap.count(matFolder + matFile) < 1)
		ReadMaterialData(matFolder, matFile);

	for (Material* material : materialMap[matFolder + matFile])	{
		Material* temp = NULL;
		material->Clone((void **)&temp);
		materials->push_back(temp);
	}
}

void Models::ReadMaterialData(wstring matFolder, wstring matFile)
{
	vector<Material *> materials;

	Xml::XMLDocument* document = new Xml::XMLDocument();

	wstring tempFile = matFolder + matFile;
	Xml::XMLError error = document->LoadFile(String::ToString(tempFile).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();
	
	do
	{
		Xml::XMLElement* node = NULL;

		Material* material = new Material();

		//Name
		node = matNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));

		//DxColor
		node = node->NextSiblingElement();

		D3DXCOLOR dxColor;
		Xml::XMLElement* color = node->FirstChildElement();

		dxColor.r = color->FloatText();
		color = color->NextSiblingElement();

		dxColor.g = color->FloatText();
		color = color->NextSiblingElement();

		dxColor.b = color->FloatText();
		color = color->NextSiblingElement();

		dxColor.a = color->FloatText();
		material->SetDiffuse(dxColor);

		//DiffuseTexture
		node = node->NextSiblingElement();
		wstring diffuseTexture = String::ToWString(node->GetText());
		if (diffuseTexture.size() > 0)
			material->SetDiffuseMap(matFolder + diffuseTexture);

		materials.push_back(material);
		matNode = matNode->NextSiblingElement();
	} while (matNode != NULL);

	materialMap[tempFile] = materials;
}

map<wstring, Models::MeshData> Models::meshDataMap;

void Models::LoadMesh(wstring MeshFolder, wstring MeshFile, vector<class ModelBone*>* bones, vector<class ModelMesh*>* meshes)
{
	if (meshDataMap.count(MeshFolder + MeshFile) < 1)
		ReadMeshData(MeshFolder, MeshFile);

	MeshData data = meshDataMap[MeshFolder + MeshFile];
	for (size_t i = 0; i < data.Bones.size(); i++) {
		ModelBone* bone = NULL;
		data.Bones[i]->Clone((void **)&bone);

		bones->push_back(bone);
	}

	for (size_t i = 0; i < data.Meshes.size(); i++) {
		ModelMesh* mesh = NULL;
		data.Meshes[i]->Clone((void**)&mesh);

		meshes->push_back(mesh);
	}
}

void Models::ReadMeshData(wstring MeshFolder, wstring MeshFile)
{
	BinaryReader* r = new BinaryReader();
	r->Open(MeshFolder + MeshFile);


	UINT count = 0;
	count = r->UInt();
	vector<ModelBone *> bones;

	for (UINT i = 0; i < count; i++) {
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = String::ToWString(r->String());
		bone->parentIndex = r->Int();
		bone->local = r->Matrix();
		bone->global = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();

	vector<ModelMesh *> meshes;
	for (UINT i = 0; i < count; i++) {
		ModelMesh* mesh = new ModelMesh();
		mesh->name = String::ToWString(r->String());
		mesh->parentBoneIndex = r->Int();

		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++) {
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = String::ToWString(r->String());

			//VertexData
			{
				UINT count = r->UInt();
				meshPart->vertices.assign(count, ModelVertexType());

				void* ptr = (void *)&(meshPart->vertices[0]);
				r->Byte(&ptr, sizeof(ModelVertexType) * count);
			}

			//IndexData
			{
				UINT count = r->UInt();
				meshPart->indices.assign(count, UINT());

				void* ptr = (void *)&(meshPart->indices[0]);
				r->Byte(&ptr, sizeof(UINT) * count);
			}

			mesh->meshParts.push_back(meshPart);
		}//for(k)
		meshes.push_back(mesh);
	}//for(i)

	r->Close();
	SAFE_DELETE(r);

	MeshData data;
	data.Bones.assign(bones.begin(), bones.end());
	data.Meshes.assign(meshes.begin(), meshes.end());

	meshDataMap[MeshFolder + MeshFile] = data;
}
