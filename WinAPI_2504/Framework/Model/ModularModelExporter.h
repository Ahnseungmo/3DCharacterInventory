#pragma once

class ModularModelExporter
{
public:
    ModularModelExporter(string name, string file);
    ~ModularModelExporter();

    // 재질과 애니메이션 클립을 익스포트하는 함수
    void ExportMaterial();
    void ExportClip(string clipName);

    // 선택된 메쉬 파츠를 익스포트하는 함수
    void ExportMeshPart(string partName, const vector<UINT>& meshIndices);

    // 외부(ImGui)에서 씬 정보에 접근하기 위한 Getter 함수
    const aiScene* GetScene() const { return scene; }
    const string& GetName() const { return name; }
    const string& GetFile() const { return file; }

private:
    // Material 관련 내부 함수
    void ReadMaterial(vector<Material*>& materials);
    void WriteMaterial(const vector<Material*>& materials);
    string CreateTexture(string file);

    // Mesh 관련 내부 함수
    void ReadNode(aiNode* node, int index, int parent, vector<NodeData*>& nodes);
    void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights,
                  map<string, UINT>& boneMap, UINT& boneCount, vector<BoneData*>& bones);
    void WriteMesh(string partName, const vector<MeshData*>& meshes,
                   const vector<NodeData*>& nodes, const vector<BoneData*>& bones);

    // Animation 관련 내부 함수
    Clip* ReadClip(aiAnimation* animation);
    void WriteClip(Clip* clip, string clipName, UINT index);
    void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);

private:
    Assimp::Importer* importer;
    const aiScene* scene;

    string name;
    string file; // 원본 FBX 파일 경로
};