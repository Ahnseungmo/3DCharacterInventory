#pragma once

class Environment : public Singleton<Environment>
{
	friend class Singleton;

private:
	Environment();
	~Environment();

public:
	void Update();
	void Edit();

	void SetViewport(UINT width = SCREEN_WIDTH, UINT height = SCREEN_HEIGHT);
	void SetRender();
	void SetPostRender();

	LightBuffer::Light* AddLight();

	LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->GetData()->lights[index]; }

	Camera* GetMainCamera() { return mainCamera; }
	Matrix GetPerspective() { return perspective; }

private:
	void CreateProjection();
	void CreateSamplerState();
	void CreateBlendState();
	void CreateStats();

	void EditLight(LightBuffer::Light& light);

private:
	bool isWireFrame = false;

	Matrix perspective;
	Matrix orthographic;

	class Grid* grid;
	Camera* mainCamera;
	MatrixBuffer* projectionBuffer;
	MatrixBuffer* uiViewBuffer;
	LightBuffer* lightBuffer;

	ID3D11SamplerState* samplerState;
	ID3D11BlendState* alphaBlendState;	

	RasterizerState* rasterizerState[2];	
};