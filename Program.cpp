#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/OrbitCamera.h"

#include "./Executes/ExportModel.h"
#include "./Executes/ExeGrid.h"

Program::Program()
{
	States::Create();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->GlobalLight = new LightBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);
	
	orbitCamera = new OrbitCamera(100.0f, 100.0f, 2.5f, 0);
	freeCamera = new Freedom(100.0f,2.5f);

	values->MainCamera = freeCamera;
	values->MainCamera->Position(0, 0, -100.0f);

	freeCamera->Position(0, 0, -100.0f);
	orbitCamera->Position(0, 0, -100.0f);

	executes.push_back(new ExeGrid(values));
	executes.push_back(new ExportModel(values));

	isOrbit = false;

}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	/*SAFE_DELETE(freeCamera);
	SAFE_DELETE(orbitCamera);*/
	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values->GlobalLight);
	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values);

	States::Delete();
}

void Program::Update()
{
	
	if (isOrbit == true)
		values->MainCamera = orbitCamera;
	else
		values->MainCamera = freeCamera;

	values->MainCamera->Update();

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXMATRIX view, projection;
	values->MainCamera->Matrix(&view);
	values->Perspective->GetMatrix(&projection);

	values->ViewProjection->SetView(view);
	values->ViewProjection->SetProjection(projection);
	values->ViewProjection->SetVSBuffer(0);
	values->GlobalLight->SetPSBuffer(0);

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	for (Execute* exe : executes)
		exe->PostRender();
	ImGui::Begin("Info");
	{
		ImGui::Text("FPS : %f", Time::Get()->FPS());
		ImGui::Checkbox("Orbit_Sphere", &isOrbit);
		ImGui::SliderFloat3("LightDirection", (float*)&values->GlobalLight->Data.Direction, -1, 1);
	}
	ImGui::End();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}