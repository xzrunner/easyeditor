#include "AssimpHelper.h"
#include "Texture.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <m3_model.h>
#include <m3_material.h>
#include <m3_typedef.h>
#include <ds_array.h>
#include <SM_Vector.h>

namespace gum
{

std::map<std::string, Texture*> AssimpHelper::m_tex_cache;

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

m3_model* AssimpHelper::Load(const char* filename)
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
		return NULL;
	}

	m3_model* model = m3_model_create(16);

	std::string dir = filename;
	int pos = dir.find_last_of('/');
	if (pos != std::string::npos) {
		dir = dir.substr(0, pos);
	} else {
		dir = "";
	}
	LoadNode(scene, scene->mRootNode, model, dir);

	// todo: load lights and cameras

	return model;
}

void AssimpHelper::LoadNode(const aiScene* ai_scene, const aiNode* ai_node, 
							m3_model* model, const std::string& dir)
{
	if (ai_node->mNumChildren) 
	{
		for (int i = 0; i < ai_node->mNumChildren; ++i) {
			LoadNode(ai_scene, ai_node->mChildren[i], model, dir);
		}
	} 
	else 
	{
		if (ai_node->mNumMeshes) 
		{
			for (int i = 0; i < ai_node->mNumMeshes; ++i) 
			{
				const aiMesh* mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
				const aiMaterial* material = ai_scene->mMaterials[mesh->mMaterialIndex];

				m3_mesh* dst_mesh = new m3_mesh;
				LoadMesh(mesh, material, dst_mesh, dir);
				ds_array_add(model->meshes, &dst_mesh);
			}
		}
	}
}

void AssimpHelper::LoadMesh(const aiMesh* ai_mesh, const aiMaterial* ai_material, 
							m3_mesh* mesh, const std::string& dir)
{
	LoadMaterial(ai_mesh, ai_material, &mesh->material, dir);

	int floats_per_vertex = 3;
	bool has_normal = ai_mesh->HasNormals();
	if (has_normal) {
		floats_per_vertex += 3;
		mesh->vertex_type |= M3_VERTEX_FLAG_NORMALS;
	}
	bool has_texcoord = ai_mesh->HasTextureCoords(0);
	if (has_texcoord) {
		floats_per_vertex += 2;
		mesh->vertex_type |= M3_VERTEX_FLAG_TEXCOORDS;
	}

	mesh->vertices = ds_array_create(ai_mesh->mNumVertices, floats_per_vertex * sizeof(float));
	float vertex[3 + 3 + 2];
	float* vertex_ptr = vertex;
	for (int i = 0; i < ai_mesh->mNumVertices; ++i) 
	{
		const aiVector3D& p = ai_mesh->mVertices[i];
		sm::vec3 position;
		position.x = p.x;
		position.y = p.y;
		position.z = p.z;
		memcpy(vertex_ptr, &position, sizeof(position));
		vertex_ptr += 3;
	
		if (has_normal) 
		{
			const aiVector3D& n = ai_mesh->mNormals[i];
			sm::vec3 normal;
			normal.x = n.x;
			normal.y = n.y;
			normal.z = n.z;
			memcpy(vertex_ptr, &normal, sizeof(normal));
			vertex_ptr += 3;
		}

		if (has_texcoord)
		{
			const aiVector3D& t = ai_mesh->mTextureCoords[0][i];
			struct sm_vec2 texcoord;
			texcoord.x = t.x;
			texcoord.y = t.y;
			memcpy(vertex_ptr, &texcoord, sizeof(texcoord));
			vertex_ptr += 2;
		}

		ds_array_add(mesh->vertices, &vertex);
		vertex_ptr = vertex;
	}

	int count = 0;
	for (int i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace& face = ai_mesh->mFaces[i];
		count += face.mNumIndices;
	}
	mesh->indices = ds_array_create(count, sizeof(unsigned short));
	for (int i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace& face = ai_mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j) {
			ds_array_add(mesh->indices, &face.mIndices[j]);
		}
	}
}

void AssimpHelper::LoadMaterial(const aiMesh* ai_mesh, const aiMaterial* ai_material, 
								m3_material* material, const std::string& dir)
{
	aiColor4D col;

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &col) == AI_SUCCESS) {
		material->diffuse.x = col.r;
		material->diffuse.y = col.g;
		material->diffuse.z = col.b;
	} else {
		material->diffuse.x = material->diffuse.y = material->diffuse.z = 1;
	}

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &col) == AI_SUCCESS) {
		material->specular.x = col.r;
		material->specular.y = col.g;
		material->specular.z = col.b;
	} else {
		material->specular.x = material->specular.y = material->specular.z = 1;
	}

	if (aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_AMBIENT, &col) == AI_SUCCESS) {
		material->ambient.x = col.r;
		material->ambient.y = col.g;
		material->ambient.z = col.b;
	} else {
		material->ambient.x = material->ambient.y = material->ambient.z = 0.04f;
	}
	material->shininess = 50;

	material->tex = -1;
	if (ai_mesh->mTextureCoords[0]) 
	{
		aiString path;
		if (aiGetMaterialString(ai_material, AI_MATKEY_TEXTURE_DIFFUSE(0), &path) == AI_SUCCESS) 
		{
			std::string img_path = dir + '/' + path.data;
			std::map<std::string, Texture*>::iterator itr = m_tex_cache.find(img_path);
			Texture* tex = NULL;
			if (itr == m_tex_cache.end()) {
				tex = new Texture;
				tex->Load(img_path);
				m_tex_cache.insert(std::make_pair(img_path, tex));
			} else {
				tex = itr->second;
			}
			material->tex = tex->GetID();
		}
	}
}

}