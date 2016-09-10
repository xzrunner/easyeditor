#ifndef _GLUE_MESH_SPR_LOADER_H_
#define _GLUE_MESH_SPR_LOADER_H_

#include <CU_Uncopyable.h>

#include <json/json.h>

namespace s2 { class MeshSprite; }

namespace glue
{

class MeshSprLoader : private cu::Uncopyable
{
public:
	MeshSprLoader(s2::MeshSprite* spr);
	~MeshSprLoader();

	void LoadJson(const Json::Value& val, const std::string& dir);

private:
	s2::MeshSprite* m_spr;

}; // MeshSprLoader

}

#endif // _GLUE_MESH_SPR_LOADER_H_