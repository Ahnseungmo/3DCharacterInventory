#pragma once
struct Triangle
{
    Vertex v0, v1, v2;
    Vector3 normal; // The normal vector for the triangle's plane.

    // Constructor to initialize the triangle
    Triangle(Vertex v0, Vertex v1, Vertex v2)
        : v0(v0), v1(v1), v2(v2)
    {
        normal = Vector3::Cross(v1.ToVector3() - v0.ToVector3(), v2.ToVector3() - v0.ToVector3()).GetNormalized();
    }
};

class MeshCollider : public Collider
{
public:
    MeshCollider(Mesh<Vertex>* mesh);
    virtual ~MeshCollider() = default;

    virtual bool IsRayCollision(const Ray& ray, RayHit* hitInfo = nullptr) override;
    bool RayIntersectsTriangle(const Ray& ray, const Triangle& triangle, float& t);

    virtual bool IsBoxCollision(BoxCollider* collider) override;
    bool IsSeperateAxis(const BoxCollider::ObbDesc& box, const Triangle& triangle, const Vector3& axis);
    void ProjectToAxis(const BoxCollider::ObbDesc& box, const Vector3& axis, float& minValue, float& maxValue);
    void ProjectTriangleToAxis(const Triangle& triangle, const Vector3& axis, float& minValue, float& maxValue);
    virtual bool IsSphereCollision(SphereCollider* collider) override;
    virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

protected:
    virtual void MakeMesh() override;

private:
    // Mesh data
    Mesh<Vertex>* mesh;
    vector<Triangle> triangles;

    // A helper function for creating triangle meshes
    void CreateTriangles();
};
