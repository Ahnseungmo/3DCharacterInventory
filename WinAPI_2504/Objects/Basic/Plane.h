#pragma once

class Plane : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Plane(Vector2 size = Vector2(10, 10));
	~Plane();

	void Render();

	Vector2 GetSize() const { return size; }

private:
	void MakeMesh();

private:
	Vector2 size;

	Mesh<VertexType>* mesh;
};