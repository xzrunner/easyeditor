#ifndef _EASYEDITOR_BODY_MANAGER_H_
#define _EASYEDITOR_BODY_MANAGER_H_

#include <map>

namespace ee
{

class IBody;
class Sprite;

class BodyManager
{
public:
	static BodyManager* Instance();

	IBody* LoadBody(Sprite* spr);
	void UnloadBody(Sprite* spr);

	const IBody* QueryBody(Sprite* spr) const;

	void Update();

private:
	BodyManager();

private:
	static IBody* CreateBody(Sprite* spr);

private:
	std::map<Sprite*, IBody*> m_map_body;

private:
	static BodyManager* m_instance;

}; // BodyManager

}

#endif // _EASYEDITOR_BODY_MANAGER_H_