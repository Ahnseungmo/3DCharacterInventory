#include "Framework.h"

MeshCollider::MeshCollider(Mesh<Vertex>* mesh) : mesh(mesh)
{
	type = Type::Mesh;

	MakeMesh(); // 메쉬로부터 삼각형을 생성
}

bool MeshCollider::IsRayCollision(const Ray& ray, RayHit* hitInfo)
{
    float tMin = FLT_MAX;
    bool hit = false;

    // 메쉬의 각 삼각형에 대해 교차 여부 확인
    for (const auto& triangle : triangles)
    {
        float t = 0;
        if (RayIntersectsTriangle(ray, triangle, t))
        {
            if (t < tMin)
            {
                tMin = t;
                hit = true;
                if (hitInfo)
                {
                    hitInfo->isHit = true;
                    hitInfo->distance = tMin;
                    hitInfo->position = ray.origin + ray.direction * tMin;
                    hitInfo->normal = triangle.normal;
                }
            }
        }
    }

    return hit;
}

bool MeshCollider::RayIntersectsTriangle(const Ray& ray, const Triangle& triangle, float& t)
{
    // Möller–Trumbore 교차 알고리즘
    const Vector3 p0 = triangle.v1.ToVector3();
    const Vector3 p1 = triangle.v1.ToVector3();
    const Vector3 p2 = triangle.v2.ToVector3();

    Vector3 e1 = p1 - p0;
    Vector3 e2 = p2 - p0;
    Vector3 h = Vector3::Cross(ray.direction, e2);
    float a = Vector3::Dot(e1, h);

    if (abs(a) < FLT_EPSILON)
        return false;

    float f = 1.0f / a;
    Vector3 s = ray.origin - p0;
    float u = f * Vector3::Dot(s, h);

    if (u < 0.0f || u > 1.0f)
        return false;

    Vector3 q = Vector3::Cross(s, e1);
    float v = f * Vector3::Dot(ray.direction, q);

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = f * Vector3::Dot(e2, q);

    if (t > FLT_EPSILON)
        return true;

    return false;
}
bool MeshCollider::IsBoxCollision(BoxCollider* collider)
{
    BoxCollider::ObbDesc box;
    collider->GetOBB(box);

    // Box의 각 축을 기준으로 분리축 정리를 적용하여 충돌 여부를 확인
    for (const auto& triangle : triangles)
    {
        Vector3 normal = triangle.normal;

        // Box의 각 축과 삼각형의 법선 벡터를 사용하여 충돌 여부를 확인
        if (IsSeperateAxis(box, triangle, normal))
            return false;
    }

    return true;
}

bool MeshCollider::IsSeperateAxis(const BoxCollider::ObbDesc& box, const Triangle& triangle, const Vector3& axis)
{
    // Box와 삼각형을 기준으로 분리축 정리를 적용한 코드
    // 이 부분에서는 Box의 각 축과 삼각형의 축들 간에 분리축 정리를 적용해
    // 충돌 여부를 계산합니다.
    float boxMin, boxMax, triangleMin, triangleMax;

    ProjectToAxis(box, axis, boxMin, boxMax);
    ProjectTriangleToAxis(triangle, axis, triangleMin, triangleMax);

    if (boxMax < triangleMin || boxMin > triangleMax)
        return true; // 충돌 없음

    return false; // 충돌 있음
}

void MeshCollider::ProjectToAxis(const BoxCollider::ObbDesc& box, const Vector3& axis, float& minValue, float& maxValue)
{
    // Box를 주어진 축으로 투영한 최소값과 최대값을 계산
    minValue = maxValue = Vector3::Dot(axis, box.center);

    for (int i = 0; i < 3; ++i)
    {
        float projection = Vector3::Dot(axis, box.axis[i]) * box.halfSize[i];
        minValue = min(minValue, minValue + projection);
        maxValue = max(maxValue, maxValue + projection);
    }
}

void MeshCollider::ProjectTriangleToAxis(const Triangle& triangle, const Vector3& axis, float& minValue, float& maxValue)
{
    // 삼각형을 주어진 축으로 투영한 최소값과 최대값을 계산
    Vector3 v0 = triangle.v0.ToVector3();
    Vector3 v1 = triangle.v1.ToVector3();
    Vector3 v2 = triangle.v2.ToVector3();

    float proj0 = Vector3::Dot(v0, axis);
    float proj1 = Vector3::Dot(v1, axis);
    float proj2 = Vector3::Dot(v2, axis);

    minValue = min(proj0, proj1, proj2);
    maxValue = max (proj0, proj1, proj2);
}

bool MeshCollider::IsSphereCollision(SphereCollider* collider)
{
	return false;
}

bool MeshCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
	return false;
}

void MeshCollider::MakeMesh()
{
	CreateTriangles(); // 삼각형 데이터를 생성하는 헬퍼 함수 호출

	// 선택적으로 메쉬의 시각화용으로 메쉬를 생성할 수도 있습니다.
	// (게임 엔진이나 렌더링 시스템에 따라 다를 수 있습니다)
	mesh->CreateMesh();
}

void MeshCollider::CreateTriangles()
{
    triangles.clear();

    // 메쉬의 정점 및 인덱스 데이터 가져오기
    const vector<Vertex>& vertices = mesh->GetVertices();
    const vector<UINT>& indices = mesh->GetIndices();

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Vertex v0 = vertices[indices[i]];
        Vertex v1 = vertices[indices[i + 1]];
        Vertex v2 = vertices[indices[i + 2]];

        triangles.emplace_back(v0, v1, v2);
    }
}