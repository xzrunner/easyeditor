#ifndef _EASYEDITOR_SPRITE_FACTORY_H_
#define _EASYEDITOR_SPRITE_FACTORY_H_

#include "Symbol.h"

#include <vector>

namespace ee
{

class Sprite;

class SpriteFactory
{
public:
	Sprite* Create(Symbol* symbol);

	void Insert(Sprite* sprite);
	void Remove(Sprite* sprite);

	void UpdateBoundings(const Symbol& symbol);

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

	int m_id;

private:
	static SpriteFactory* m_instance;

	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap m_creators;

}; // SpriteFactory

}

#endif // _EASYEDITOR_SPRITE_FACTORY_H_