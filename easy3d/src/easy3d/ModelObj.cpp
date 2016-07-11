#include <gl/glew.h>

#include "ModelObj.h"
#include "ISurface.h"

#include <wx/log.h>

#include <fstream>
#include <sstream>

#include <assert.h>

namespace e3d 
{

ModelObj::ModelObj(const char* filename, float scale /*= 1.0f*/)
	: _scale(scale)
{
	Import(filename);
}

void ModelObj::Import(const char* filename)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	assert(fin);

	while (fin) {
		std::string line;
		getline(fin, line);
		if (line.empty()) continue;

		std::vector<std::string> tokens;
		SplitString(line, tokens);

		const std::string& tag = tokens[0];
		if (tag == "mtllib")
		{
			if (tokens.size() < 2) return Error(fin, filename);

			std::string path(filename);

			int a = path.rfind('/');
			int b = path.rfind('\\');
			std::string dir = path.substr(0, std::max(a, b) + 1);
			HandleMaterial((dir + tokens[1]).c_str());
		}
		else if (tag == "g")
		{
			if (tokens.size() < 2) return Error(fin, filename);
			HandleObject();
			_curr_object.name = tokens[1];
		}
		else if (tag == "usemtl")
		{
			if (tokens.size() < 2) return Error(fin, filename);
			HandleMesh();
			_curr_mesh.material_id = -1;
			for (size_t i = 0, n = _materials.size(); i < n; ++i) {
				if (_materials[i].name == tokens[1]) {
					_curr_mesh.material_id = i;
					break;
				}
			}
		}
		else if (tag == "v")
		{
			if (tokens.size() < 4) return Error(fin, filename);
			HandleVertice(tokens);
		}
		else if (tag == "f")
		{
			if (tokens.size() < 4) return Error(fin, filename);
			HandleFace(tokens);
		}
	}
	HandleObject();

	fin.close();

	InitNormals();
	InitAllMeshes();
}

void ModelObj::InitNormals()
{
	_normals.resize(_vertices.size(), sm::vec3(0, 0, 0));
	for (size_t faceIndex = 0; faceIndex < _faces.size(); ++faceIndex) {
		sm::ivec3 face = _faces[faceIndex];

		// Compute the facet normal.
		sm::vec3 a = _vertices[face.x];
		sm::vec3 b = _vertices[face.y];
		sm::vec3 c = _vertices[face.z];
		sm::vec3 facetNormal = (b - a).Cross(c - a);

		// Add the facet normal to the lighting normal of each adjoining vertex.
		_normals[face.x] += facetNormal;
		_normals[face.y] += facetNormal;
		_normals[face.z] += facetNormal;
	}

	// Normalize the normals.
	for (size_t i = 0, n = _normals.size(); i < n; ++i)
		_normals[i].Normalize();

	// Copy to MeshInfo
	for (size_t i = 0, n = _objects.size(); i < n; ++i) {
		for (size_t j = 0, m = _objects[i].meshes.size(); j < m; ++j) {
			MeshInfo& mesh = _objects[i].meshes[j];
			mesh.normals.resize(mesh.vertices.size(), sm::vec3());
			std::map<int, int>::iterator itr = mesh.map_index.begin();
			for ( ; itr != mesh.map_index.end(); ++itr)
				mesh.normals[itr->second] = _normals[itr->first];
		}
	}
}

void ModelObj::InitAllMeshes()
{
	for (size_t i = 0, n = _objects.size(); i < n; ++i) {
		for (size_t j = 0, m = _objects[i].meshes.size(); j < m; ++j) {
			const MeshInfo& mesh_data = _objects[i].meshes[j];
			const MaterialInfo& material_data = _materials[mesh_data.material_id];

			Mesh mesh;

			mesh.material.ambient = material_data.ambient;
			mesh.material.diffuse = material_data.diffuse;
			mesh.material.specular = material_data.specular;

			// Create the VBO for the vertices.
			std::vector<float> vertices;
			mesh_data.GenerateVertices(vertices, VertexFlagsNormals);
			GLuint vertex_buffer;
			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(
				GL_ARRAY_BUFFER,
				vertices.size() * sizeof(vertices[0]),
				&vertices[0],
				GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Create a new VBO for the indices if needed.
			int index_count = mesh_data.GetTriangleIndexCount();
			GLuint index_buffer;

			std::vector<GLushort> indices(index_count);
			mesh_data.GenerateTriangleIndices(indices);
			glGenBuffers(1, &index_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER,
				index_count * sizeof(GLushort),
				&indices[0],
				GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

 			mesh.index_buffer = index_buffer;
 			mesh.vertex_buffer = vertex_buffer;
 			mesh.index_count = index_count;

			_meshes.push_back(mesh);
		}
	}
}

void ModelObj::HandleMaterial(const char* filename)
{
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	assert(fin);

	MaterialInfo material;

	while (fin) {
		std::string line;
		getline(fin, line);
		if (line.empty()) continue;

		std::vector<std::string> tokens;
		SplitString(line, tokens);

		const std::string& tag = tokens[0];
		if (tag == "newmtl")
		{
			if (tokens.size() < 2) return Error(fin, filename);

			if (!material.name.empty())
				_materials.push_back(material);
			material.clear();
			material.name = tokens[1];
		}
		else if (tag == "Ka")
		{
			if (tokens.size() < 4) return Error(fin, filename);
			material.ambient = String2Vec3(tokens[1], tokens[2], tokens[3]);
		}
		else if (tag == "Kd")
		{
			if (tokens.size() < 4) return Error(fin, filename);
			material.diffuse = String2Vec3(tokens[1], tokens[2], tokens[3]);
		}
		else if (tag == "Ks")
		{
			if (tokens.size() < 4) return Error(fin, filename);
			material.specular = String2Vec3(tokens[1], tokens[2], tokens[3]);
		}
	}

	if (!material.name.empty())
		_materials.push_back(material);

	fin.close();
}

void ModelObj::HandleObject()
{
	HandleMesh();
	if (!_curr_object.meshes.empty())
	{
		_objects.push_back(_curr_object);
		_curr_object.clear();
	}
}

void ModelObj::HandleMesh()
{
	if (!_curr_mesh.faces.empty())
	{
		_curr_object.meshes.push_back(_curr_mesh);
		_curr_mesh.clear();
	}
}

void ModelObj::HandleVertice(const std::vector<std::string>& tokens)
{
	sm::vec3 pos = String2Vec3(tokens[1], tokens[2], tokens[3]);
	_vertices.push_back(pos * _scale);
}

void ModelObj::HandleTextureCoordinate(const std::vector<std::string>& tokens)
{
	
}

void ModelObj::HandleNormal(const std::vector<std::string>& tokens)
{
	
}

void ModelObj::HandleFace(const std::vector<std::string>& tokens)
{
	// split
	if (tokens.size() > 4)
	{
		size_t size = tokens.size()-1;
		std::vector<uint16_t> nodes;
		nodes.resize(size);
		for (size_t i = 0; i < size; ++i)
		{
			std::vector<std::string> elems;
			SplitString(tokens[1+i], elems, '/');
			nodes[i] = atoi(elems[0].c_str());
		}

//		if (size % 2 == 0) {
//// 			for (size_t i = 0, n = size/2-1; i < n; ++i)
//// 			{
//// 				int index = i * 2;
//// 
//// 				_curr_mesh.AddFace(*this, nodes[index], nodes[index+1], nodes[index+2]);
//// 				_curr_mesh.AddFace(*this, nodes[index], nodes[index+2], nodes[index+3]);
//// 
//// 				if (i == 0) break;
//// 			}
//
//			// fan
// 			for (size_t i = 2; i < size; ++i)
// 			{
// 				_curr_mesh.AddFace(*this, nodes[0], nodes[i-1], nodes[i]);
// 			}
//
//		} else {
 			for (size_t i = 2; i < size; ++i)
 				_curr_mesh.AddFace(*this, nodes[0], nodes[i-1], nodes[i]);
//		}
	}
	else
	{
		uint16_t nodes[3];
		for (size_t i = 0; i < 3; ++i)
		{
			std::vector<std::string> elems;
			SplitString(tokens[1+i], elems, '/');
			nodes[i] = atoi(elems[0].c_str());
		}

		_curr_mesh.AddFace(*this, nodes[0], nodes[1], nodes[2]);
	}
}

void ModelObj::Error(std::ifstream& fin, const char* filename)
{
	fin.close();
	wxLogDebug("load %s fail", filename);
}

void ModelObj::SplitString(const std::string& src, std::vector<std::string>& dst)
{
	dst.clear();
	std::istringstream iss(src);
	copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter<std::vector<std::string> >(dst));
}

void ModelObj::SplitString(const std::string& src, std::vector<std::string>& dst, char delim)
{
	std::stringstream ss(src);
	std::string item;
	while (std::getline(ss, item, delim)) {
		dst.push_back(item);
	}
}

sm::vec3 ModelObj::String2Vec3(const std::string& sx, const std::string& sy, const std::string& sz)
{
	sm::vec3 ret;
	ret.x = static_cast<float>(atof(sx.c_str()));
	ret.y = static_cast<float>(atof(sy.c_str()));
	ret.z = static_cast<float>(atof(sz.c_str()));
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// class ModelObj::MeshInfo
//////////////////////////////////////////////////////////////////////////

int ModelObj::MeshInfo::
GetVertexCount() const
{
	return vertices.size();
}

int ModelObj::MeshInfo::
GetTriangleIndexCount() const
{
	return faces.size() * 3;
}

void ModelObj::MeshInfo::
GenerateVertices(std::vector<float>& floats, unsigned char flags) const
{
	struct Vertex {
		sm::vec3 Position;
		sm::vec3 Normal;
	};

	// Read in the vertex positions and initialize lighting normals to (0, 0, 0).
	floats.resize(GetVertexCount() * 6);
	Vertex* vertex = (Vertex*) &floats[0];
	for (size_t i = 0, n = vertices.size(); i < n; ++i, ++vertex)
	{
		vertex->Position = vertices[i];
		vertex->Normal = normals[i];
	}
}

void ModelObj::MeshInfo::
GenerateTriangleIndices(std::vector<unsigned short>& indices) const
{
	indices.resize(GetTriangleIndexCount());
	std::vector<unsigned short>::iterator index = indices.begin();
	for (std::vector<sm::ivec3>::const_iterator f = faces.begin(); f != faces.end(); ++f) {
		*index++ = f->x;
		*index++ = f->y;
		*index++ = f->z;
	}
}

void ModelObj::MeshInfo::
AddFace(ModelObj& obj, int i0, int i1, int i2)
{
	i0 -= 1;
	i1 -= 1;
	i2 -= 1;

	obj._faces.push_back(sm::ivec3(i0, i1, i2));
	i0 = AddNode(obj, i0);
	i1 = AddNode(obj, i1);
	i2 = AddNode(obj, i2);
	faces.push_back(sm::ivec3(i0, i1, i2));
}

int ModelObj::MeshInfo::
AddNode(ModelObj& obj, int index)
{
	std::map<int, int>::iterator itr = map_index.find(index);
	if (itr != map_index.end()) {
		return itr->second;
	} else {
		vertices.push_back(obj._vertices[index]);
		int ret = map_index.size();
		map_index.insert(std::make_pair(index, ret));
		return ret;
	}
}

}	