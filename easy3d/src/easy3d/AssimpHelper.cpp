#include <gl/glew.h>

#include "AssimpHelper.h"
#include "IModel.h"
#include "ModelParametric.h"
#include "AABB.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace e3d
{

// default pp steps
unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
	aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
	aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
	aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
	aiProcess_RemoveRedundantMaterials | // remove redundant materials
	aiProcess_FindDegenerates          | // remove degenerated polygons from the import
	aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
	aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
	aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
	aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
	aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
	aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
	aiProcess_SplitByBoneCount         | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
	0;

void AssimpHelper::LoadFile(const char* filename, ModelParametric& model, e3d::AABB& aabb)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		ppsteps | /* configurable pp steps */
		aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
		aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
		aiProcess_ConvertToLeftHanded	   | // convert everything to D3D left handed space
		aiProcess_SortByPType                // make 'clean' meshes which consist of a single typ of primitives
		);

	if (!scene) {
		return;
	}

	LoadNode(scene, scene->mRootNode, model, aabb);

	// todo: load lights and cameras
}

void AssimpHelper::LoadNode(const aiScene* scene, const aiNode* node, 
							ModelParametric& model, e3d::AABB& aabb)
{
	if (node->mNumChildren) {
		for (int i = 0; i < node->mNumChildren; ++i) {
			LoadNode(scene, node->mChildren[i], model, aabb);
		}
	} else {
		if (node->mNumMeshes) {
			for (int i = 0; i < node->mNumMeshes; ++i) {
				const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				LoadMesh(mesh, material, model, aabb);
			}
		}
	}
}

void AssimpHelper::LoadMesh(const aiMesh* ai_mesh, const aiMaterial* material, ModelParametric& model, e3d::AABB& aabb)
{
	Mesh mesh;

	LoadMaterial(ai_mesh, material, mesh.material);

	// Create the VBO for the vertices.
	std::vector<sm::vec3> vertices;
	bool has_texcoords = ai_mesh->HasTextureCoords(0);
	if (has_texcoords) {
		vertices.reserve(ai_mesh->mNumVertices * 3);
	} else {
		vertices.reserve(ai_mesh->mNumVertices * 2);
	}
	for (int i = 0; i < ai_mesh->mNumVertices; ++i) {
		const aiVector3D& p = ai_mesh->mVertices[i];
		sm::vec3 _p(p.x, p.y, p.z);
		vertices.push_back(_p);
		aabb.Combine(_p);

		const aiVector3D& n = ai_mesh->mNormals[i];
		vertices.push_back(sm::vec3(n.x, n.y, n.z));

		if (has_texcoords) {
			const aiVector3D& t = ai_mesh->mTextureCoords[0][i];
			vertices.push_back(sm::vec3(t.x, t.y, t.z));
		}
	}

	GLuint buf_id;
	glGenBuffers(1, &buf_id);
	mesh.vertex_buffer = buf_id;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(sm::vec3),
		&vertices[0].x,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a new VBO for the indices if needed.
	std::vector<GLushort> indices;
	for (int i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace& face = ai_mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}
	glGenBuffers(1, &buf_id);
	mesh.index_buffer = buf_id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLushort),
		&indices[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mesh.index_count = indices.size();

	model.AddMesh(mesh);
}

void AssimpHelper::LoadMaterial(const aiMesh* ai_mesh, const aiMaterial* ai_material, Material& material)
{
	aiColor4D col;

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &col) == AI_SUCCESS) {
		material.diffuse.x = col.r;
		material.diffuse.y = col.g;
		material.diffuse.z = col.b;
	} else {
		material.diffuse.x = material.diffuse.y = material.diffuse.z = 1;
	}

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &col) == AI_SUCCESS) {
		material.specular.x = col.r;
		material.specular.y = col.g;
		material.specular.z = col.b;
	} else {
		material.specular.x = material.specular.y = material.specular.z = 1;
	}

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_AMBIENT, &col) == AI_SUCCESS) {
		material.ambient.x = col.r;
		material.ambient.y = col.g;
		material.ambient.z = col.b;
	} else {
		material.ambient.x = material.ambient.y = material.ambient.z = 0;
	}

	if (ai_mesh->mTextureCoords[0]) 
	{
		aiString path;
		if (aiGetMaterialString(ai_material, AI_MATKEY_TEXTURE_DIFFUSE(0), &path) == AI_SUCCESS) {
			material.m_diffuse_tex.LoadFromFile(path.data);
		}
	}
}

}