#pragma once

class ModularModelExporter
{
public:
    ModularModelExporter(string name, string file);
    ~ModularModelExporter();

    // ������ �ִϸ��̼� Ŭ���� �ͽ���Ʈ�ϴ� �Լ�
    void ExportMaterial();
    void ExportClip(string clipName);

    // ���õ� �޽� ������ �ͽ���Ʈ�ϴ� �Լ�
    void ExportMeshPart(string partName, const vector<UINT>& meshIndices);

    // �ܺ�(ImGui)���� �� ������ �����ϱ� ���� Getter �Լ�
    const aiScene* GetScene() const { return scene; }
    const string& GetName() const { return name; }
    const string& GetFile() const { return file; }

private:
    // Material ���� ���� �Լ�
    void ReadMaterial(vector<Material*>& materials);
    void WriteMaterial(const vector<Material*>& materials);
    string CreateTexture(string file);

    // Mesh ���� ���� �Լ�
    void ReadNode(aiNode* node, int index, int parent, vector<NodeData*>& nodes);
    void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights,
                  map<string, UINT>& boneMap, UINT& boneCount, vector<BoneData*>& bones);
    void WriteMesh(string partName, const vector<MeshData*>& meshes,
                   const vector<NodeData*>& nodes, const vector<BoneData*>& bones);

    // Animation ���� ���� �Լ�
    Clip* ReadClip(aiAnimation* animation);
    void WriteClip(Clip* clip, string clipName, UINT index);
    void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);

private:
    Assimp::Importer* importer;
    const aiScene* scene;

    string name;
    string file; // ���� FBX ���� ���
};