#include "stdafx.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(float distance, float moveSpeed, float rotationSpeed, D3DXVECTOR3* target)
	: distance(distance)
	, targetPosition(target)
	, moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
{
	if (targetPosition == NULL)
		Position(0, 0, distance);
	else
		Position(targetPosition->x, targetPosition->y, targetPosition->z + distance);
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update()
{
	//Rotation
	{
		D3DXVECTOR2 rotation;
		Rotation(&rotation);
		

		if (Keyboard::Get()->Press('W'))
			rotation.x += rotationSpeed * Time::Delta();

		else if (Keyboard::Get()->Press('S'))
			rotation.x -= rotationSpeed * Time::Delta();

		if (Keyboard::Get()->Press('A'))
			rotation.y += rotationSpeed * Time::Delta();

		else if (Keyboard::Get()->Press('D'))
			rotation.y -= rotationSpeed * Time::Delta();

		Rotation(rotation.x, rotation.y);
	}

	//Translation
	{
		D3DXVECTOR3 position(0, 0, 0);
		if(targetPosition!=NULL)
			Position(targetPosition->x, targetPosition->y, targetPosition->z + 100);

		if (Keyboard::Get()->Press('Q')) {
			distance -= moveSpeed * Time::Delta();
			if (distance <= 3.0f)
				distance = 3.0f;
		}
		else if (Keyboard::Get()->Press('E'))
			distance += moveSpeed * Time::Delta();

		position.z = -distance;
		D3DXVec3TransformCoord(&position, &position, &matRotation);	//vereinigen Matrix mit Vector, danach einfügen da in Vector.

		Position(position.x, position.y, position.z);
	}
}

void OrbitCamera::View()
{
	D3DXVECTOR3 position, up;
	Position(&position);
	Up(&up);

	D3DXMatrixLookAtLH(&matView, &position, &D3DXVECTOR3(0, 0, 0), &up);
}
