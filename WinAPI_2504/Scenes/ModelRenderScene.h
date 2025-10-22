#pragma once

class ModelRenderScene : public Scene
{
public:
	ModelRenderScene();
	~ModelRenderScene();	

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Model* model;
};