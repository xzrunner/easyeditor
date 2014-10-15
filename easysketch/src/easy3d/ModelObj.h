#ifndef _E3D_MODEL_OBJ_H_
#define _E3D_MODEL_OBJ_H_

#include "IModel.h"

namespace e3d 
{

class ModelObj : public IModel
{
public:
	ModelObj(const char* filename, float scale = 1.0f);

	virtual const std::vector<Mesh>& GetAllMeshes() const {
		return _meshes;
	}

private:
	void Import(const char* filename);
	void InitNormals();
	void InitAllMeshes();

	void HandleMaterial(const char* filename);
	void HandleObject();
	void HandleMesh();
	void HandleVertice(const std::vector<std::string>& tokens);
	void HandleTextureCoordinate(const std::vector<std::string>& tokens);
	void HandleNormal(const std::vector<std::string>& tokens);
	void HandleFace(const std::vector<std::string>& tokens);

	void Error(std::ifstream& fin, const char* filename);

	static void SplitString(const std::string& src, std::vector<std::string>& dst);
	static void SplitString(const std::string& src, std::vector<std::string>& dst, char delim);

	static vec3 String2Vec3(const std::string& sx, const std::string& sy, const std::string& sz);

private:
	struct MaterialInfo
	{
		std::string name;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		void clear() {
			name = "";
			ambient = diffuse = specular = vec3(0, 0, 0);
		}
	};

	struct MeshInfo
	{
		int material_id;
		bool has_texture;
		bool has_normal;

		std::map<int, int> map_index;
		std::vector<vec3> vertices;
		std::vector<vec3> normals;
		std::vector<ivec3> faces;

		void clear() {
			map_index.clear();
			vertices.clear();
			faces.clear();
		}

		int GetVertexCount() const;
	 	int GetTriangleIndexCount() const;
		void GenerateVertices(std::vector<float>& vertices, unsigned char flags) const;
	 	void GenerateTriangleIndices(std::vector<unsigned short>& indices) const;

		void AddFace(ModelObj& obj, int i0, int i1, int i2);
		int AddNode(ModelObj& obj, int index);
	};

	struct ObjectInfo
	{
		std::string name;
		std::vector<MeshInfo> meshes;

		void clear() {
			meshes.clear();
		}
	};

private:
	float _scale;

	std::vector<vec3> _vertices;
	std::vector<ivec3> _faces;
	std::vector<vec3> _normals;
//	std::vector<uint16> indices;

	std::vector<MaterialInfo> _materials;
	std::vector<ObjectInfo> _objects;

	ObjectInfo _curr_object;
	MeshInfo _curr_mesh;

	std::vector<Mesh> _meshes;

}; // ModelObj

}

#endif // _E3D_MODEL_OBJ_H_