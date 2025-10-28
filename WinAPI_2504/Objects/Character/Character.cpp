#include "Framework.h"

Character::Character()
{
	tag = "CharacterCapsule";

	model = new ModelAnimator("LowPolyCharacter");
//	model = new ModelAnimator("Character");
	//	model->SetShader(L"Light/LightPalette.hlsl");
	model->ReadClip("Idle");
	model->ReadClip("Run");
	model->ReadClip("Attack");
	model->CreateTexture();

	model->SetParent(this);
	model->Load();
	
	model->GetClip(Attack)->SetEvent(bind(&Character::EndAttack, this), 0.9f);

//	sword = new Sword();
	rightHand = new Transform();
//	rightHand->SetParent(this);

//	sword->SetParent(rightHand);
}

Character::~Character()
{
	delete model;
	delete sword;
	delete rightHand;
}

void Character::Update()
{
	rightHand->SetWorld(model->GetTransformByNode(36));
//	rightHand->SetWorld(model->GetTransformByNode(28));

	Move();
	StartAttack();

	model->Update();
	UpdateWorld();
	if (this->sword != NULL)
		sword->Update();
}

void Character::Render()
{
	model->Render();
	Collider::Render();
	if (this->sword != NULL)
		sword->Render();
}

void Character::Edit()
{
	Transform::Edit();
	model->Edit();
	if (this->sword != NULL)
		sword->Edit();
}

void Character::ChangeSword(Sword* sword)
{
	if(this->sword != NULL)
		this->sword->SetActive(false);
	this->sword = sword;
	this->sword->SetParent(rightHand);
	this->sword->SetActive(true);

}

void Character::Move()
{
	if (state == Attack)
		return;

	moveDir = Vector3::Zero();

	if (Input::Get()->IsKeyPress('W'))
	{
		moveDir += Vector3::Forward();
	}
	else if (Input::Get()->IsKeyPress('S'))
	{
		moveDir += Vector3::Back();
	}

	if (Input::Get()->IsKeyPress('A'))
	{
		moveDir += Vector3::Left();
	}
	else if (Input::Get()->IsKeyPress('D'))
	{
		moveDir += Vector3::Right();
	}

	if (moveDir.Length() > 0.0f)
	{
		moveDir.Normalize();	
		Translate(moveDir * moveSpeed * DELTA);
		Rotate();

		PlayClip(Run);
	}
	else
	{
		PlayClip(Idle);
	}
}

void Character::Rotate()
{
	Vector3 lookDir = moveDir;
	lookDir.y = 0.0f;
	lookDir.Normalize();
	Vector3 forward = GetForward();
	forward.y = 0.0f;
	forward.Normalize();
	float dot = Vector3::Dot(forward, lookDir);
	dot = GameMath::Clamp(dot, -1.0f, 1.0f);
	float angle = acosf(dot);
	Vector3 cross = Vector3::Cross(forward, lookDir);

	if (cross.y < 0.0f)
		angle = -angle;

	Transform::Rotate(Vector3::Up(), angle * rotateSpeed * DELTA);
}

void Character::StartAttack()
{
	if (Input::Get()->IsKeyDown(VK_LBUTTON))
		PlayClip(Attack);
}

void Character::EndAttack()
{
	PlayClip(Idle);
}

void Character::PlayClip(State state)
{
	if (this->state == state)
		return;

	this->state = state;
	model->PlayClip((int)state);
}
