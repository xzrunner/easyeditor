#ifndef _EASYEDITOR_BODY_MANAGER_H_
#define _EASYEDITOR_BODY_MANAGER_H_

#include "Sprite.h"

#include <map>

namespace ee
{

class IBody;

class BodyManager
{
public:
	static BodyManager* Instance();

	IBody* LoadBody(const SprPtr& spr);
	void UnloadBody(const SprPtr& spr);

	const IBody* QueryBody(const SprPtr& spr) const;

	void Update();

private:
	BodyManager();

private:
	static IBody* CreateBody(const SprPtr& spr);

private:
	std::map<Sprite*, IBody*> m_map_body;

private:
	static BodyManager* m_instance;

}; // BodyManager

}

#endif // _EASYEDITOR_BODY_MANAGER_H_