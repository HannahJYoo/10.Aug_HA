#include "stdafx.h"
#include "ExportModel.h"
#include "../FBX/Exporter.h"
#include "../Objects/GameModel.h"
#include "./Aufgaben/Collision.h"
ExportModel::ExportModel(ExecuteValues * values)
	:Execute(values)
{
	//Fbx::Exporter* exporter = NULL;

	
	tank = new GameModel(Models + L"/Tank/", L"Tank.material", Models + L"/Tank/", L"Tank.mesh");
	
	//objeckten.reserve(256);

	for (int i = 0; i < 50; i++) {
		objeckten.push_back(new GameModel(Models + L"/Cube/", L"Cube.material", Models + L"/Cube/", L"Cube.mesh"));
		objeckten[i]->Position(D3DXVECTOR3(Math::Random(-128, 128), 0, Math::Random(-128, 128)));
	}
	for (int i = 50; i < 100; i++) {
		objeckten.push_back(new GameModel(Models + L"/Sphere/", L"Sphere.material", Models + L"/Sphere/", L"Sphere.mesh"));
		objeckten[i]->Position(D3DXVECTOR3(Math::Random(-128, 128), 0, Math::Random(-128, 128)));
	}
	
	
}

ExportModel::~ExportModel()
{
	std::vector<GameModel*>::iterator objIter = objeckten.begin();
	while (objIter != objeckten.end()) {
		SAFE_DELETE(*objIter);
		objIter = objeckten.erase(objIter);
	}

	SAFE_DELETE(tank);

}

void ExportModel::Update()
{
	//Tank Bewegung
	{
		D3DXVECTOR3 pos = tank->Position();
		D3DXVECTOR3 dir = tank->Direction();
		float speed = 30.0f;
		float radian = 100.0f;
		D3DXVECTOR3 up = tank->Up();
		D3DXMATRIX matRotation;

		if (Keyboard::Get()->Press(VK_UP))
			tank->Position(pos - (dir * speed * Time::Get()->Delta()));
		else if (Keyboard::Get()->Press(VK_DOWN))
			tank->Position(pos + (dir * speed * Time::Get()->Delta()));

		if (Keyboard::Get()->Press(VK_LEFT)) {
			D3DXMatrixRotationAxis(&matRotation, &-up, Math::ToRadian(radian)*Time::Get()->Delta());
			tank->World(matRotation* tank->World());
		}
		if (Keyboard::Get()->Press(VK_RIGHT)) {
			D3DXMatrixRotationAxis(&matRotation, &up, Math::ToRadian(radian)*Time::Get()->Delta());
			tank->World(matRotation* tank->World());
		}
	}

	tank->Update();

	for (GameModel* objeckt : objeckten)
		objeckt->Update();

	std::vector<GameModel*>::iterator objIter = objeckten.begin();
	while (objIter != objeckten.end()) {
		bool bIntersect = false;
		bIntersect = Collision::AABBCollision(*tank, *(*objIter));

		if (bIntersect)
			objIter = objeckten.erase(objIter);
		else
			objIter++;
	}

}

void ExportModel::PreRender()
{
	
}

void ExportModel::Render()
{
	tank->Render();
	for (GameModel* objeckt : objeckten)
		objeckt->Render();
}

void ExportModel::PostRender()
{
	
}

void ExportModel::ResizeScreen()
{
}
