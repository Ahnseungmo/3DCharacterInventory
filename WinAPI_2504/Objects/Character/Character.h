#pragma once

class Character : public CapsuleCollider
{
private:
	enum State
	{
		Idle,
		Run,
		Attack,
	};

public:
	Character();
	~Character();

	void Update();
	void Render();
	void Edit();
	void ChangeSword(Sword* sword);

private:
	void Move();
	void Rotate();
	void StartAttack();

	void EndAttack();

private:
	void PlayClip(State state);

private:
	float moveSpeed = 5.0f;
	float rotateSpeed = 10.0f;

	State state = Idle;

	Vector3 moveDir = Vector3::Zero();

	ModelAnimator* model;
	Sword* sword;

	Transform* rightHand;
};