#include "Framework.h"

Camera::Camera()
{
	tag = "Camera";

	viewBuffer = new ViewBuffer();
}

Camera::~Camera()
{
	delete viewBuffer;
}

void Camera::Update()
{
	projection = Environment::Get()->GetPerspective();

    if (target)
        FollowMode();
    else
	    FreeMode();

	UpdateWorld();	

	view = XMMatrixInverse(nullptr, world);
	viewBuffer->Set(view, world);
	viewBuffer->SetVS(1);
}

void Camera::Edit()
{
	ImGui::DragFloat("MoveSpeed", &moveSpeed, 0.1f, 0.1f, 100.0f);
	ImGui::DragFloat("RotSpeed", &rotSpeed, 0.1f, 0.1f, 100.0f);

    Transform::Edit();
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
    Vector3 screenSize(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);    

    Vector2 point;
	point.x = (screenPos.x / screenSize.x) * 2.0f - 1.0f;
	point.y = (screenPos.y / screenSize.y) * 2.0f - 1.0f;
	//-> 0~screen -> -1~1

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

	screenPos.x = point.x / temp._11;
	screenPos.y = point.y / temp._22;
    screenPos.z = 1.0f;

    screenPos = XMVector3TransformNormal(screenPos, world);

    Ray ray;
	ray.origin = localPosition;
	ray.direction = screenPos.GetNormalized();

    return ray;
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (Input::Get()->IsKeyPress(VK_RBUTTON))
    {
        if (Input::Get()->IsKeyPress('W'))
            Translate(forward * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('S'))
            Translate(-forward * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('E'))
            Translate(up * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('Q'))
            Translate(-up * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('A'))
            Translate(-right * moveSpeed * DELTA);
        if (Input::Get()->IsKeyPress('D'))
            Translate(right * moveSpeed * DELTA);

        Rotate(Vector3::Right(), -delta.y * rotSpeed * DELTA);
        Rotate(Vector3::Up(), delta.x * rotSpeed * DELTA);
    }
}

void Camera::FollowMode()
{
    //Vector2 targetPos = target->GetGlobalPosition() - targetOffset;
    //FixPosition(targetPos);
    //localPosition = GameMath::Lerp<Vector2>(localPosition, targetPos, speed * DELTA);
}
