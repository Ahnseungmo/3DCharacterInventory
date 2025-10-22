#pragma once

class Camera : public Transform
{
public:
	Camera();
	~Camera();

	void Update();
	void Edit();

	void SetTarget(Transform* target) { this->target = target; }

	class Ray ScreenPointToRay(Vector3 screenPos);

private:
	void FreeMode();
	void FollowMode();

private:
	float moveSpeed = 10.0f;
	float rotSpeed = 10.0f;
	Vector3 prevMousePos;

	Matrix view;
	Matrix projection;
	ViewBuffer* viewBuffer;	

	Transform* target = nullptr;	
};