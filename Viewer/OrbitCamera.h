#pragma once

#include "Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera(float distance = 5.0f, float moveSpeed = 20.0f, float rotationSpeed = 2.5f, D3DXVECTOR3* target = NULL);
	~OrbitCamera();

	void Update();
	void View() override;

private:
	D3DXVECTOR3* targetPosition;

	float distance;
	float moveSpeed;
	float rotationSpeed;
};