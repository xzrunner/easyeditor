#ifndef _EASYEDITOR_SPRITE_FACTORY_H_
#define _EASYEDITOR_SPRITE_FACTORY_H_

#include "Symbol.h"
#include "Sprite.h"

#include <cu/cu_macro.h>

#include <json/json.h>

#include <vector>

namespace bsn { class NodeSpr; }

namespace ee
{

class SpriteFactory
{
public:
	SprPtr Create(const SymPtr& sym);
	SprPtr Create(const Json::Value& val, const std::string& dir);
	SprPtr Create(const Json::Value& val, const std::string& dir, const std::string& filepath);
	SprPtr Create(const bsn::NodeSpr* node_spr, const std::string& dir);

	SprPtr CreateRoot(const SymPtr& sym);

	std::string NextName() const;

	typedef SprPtr (*CreateCallback)(const SymPtr&);
	static void RegisterCreator(int type, CreateCallback cb);
	static void UnregisterCreator(int type);

private:
	mutable int m_id;

private:
	typedef std::map<int, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

	CU_SINGLETON_DECLARATION(SpriteFactory);

}; // SpriteFactory

}

#endif // _EASYEDITOR_SPRITE_FACTORY_H_