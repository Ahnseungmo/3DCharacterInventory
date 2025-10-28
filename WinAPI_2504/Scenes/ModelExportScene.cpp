#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	
//	string name = "Naruto";
//	string name = "Character";
	string name = "LowPolySword";
	//	string file = "Resources/Models/FBX/" + name + ".obj";
	string file = "Resources/Models/FBX/" + name + ".fbx";

	ModelExporter* exporter;

	exporter = new ModelExporter(name, file);
	exporter->ExportMaterial();
	exporter->ExportMesh();
	delete exporter;

	/*
	string clipName = "Attack";
	file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";
	
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clipName);
	delete exporter;
	
	clipName = "Run";
	file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";
	
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clipName);
	delete exporter;
	
	clipName = "Idle";
	file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";
	
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clipName);
	delete exporter;
	*/

}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
