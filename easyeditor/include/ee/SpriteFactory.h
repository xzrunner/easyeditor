#ifndef _EASYEDITOR_SPRITE_FACTORY_H_
#define _EASYEDITOR_SPRITE_FACTORY_H_

#include "Symbol.h"

#include <json/json.h>

#include <vector>

namespace ee
{

class Sprite;

class SpriteFactory
{
public:
	Sprite* Create(Symbol* sym);
	Sprite* Create(const Json::Value& val, const std::string& dir);

	void Insert(Sprite* spr);
	void Remove(Sprite* spr);

	std::string NextName() const;

	void UpdateBoundings(const Symbol& sym);

	typedef Sprite* (*CreateCallback)(Symbol*);
	static void RegisterCreator(const std::string& type, CreateCallback cb);
	static void UnregisterCreator(const std::string& type);

public:
	static SpriteFactory* Instance();

private:
	SpriteFactory();

private:
	typedef std::vector<Sprite*> SpriteList;
	std::map<const Symbol*, SpriteList> m_map_symbol2sprites;

	mutable int m_id;

private:
	static SpriteFactory* m_instance;

	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

}; // SpriteFactory

}

#endif // _EASYEDITOR_SPRITE_FACTORY_H_