#ifndef _EPHYSICS_BODY_MANAGER_H_
#define _EPHYSICS_BODY_MANAGER_H_



namespace ephysics
{

class IBody;

class BodyManager
{
public:
	static BodyManager* Instance();

	IBody* LoadBody(ee::Sprite* sprite);
	void UnloadBody(ee::Sprite* sprite);

	const IBody* QueryBody(ee::Sprite* sprite) const;

	void Update();

private:
	BodyManager();

private:
	static IBody* CreateBody(ee::Sprite* sprite);

private:
	std::map<ee::Sprite*, IBody*> m_map_body;

private:
	static BodyManager* m_instance;

}; // BodyManager

}

#endif // _EPHYSICS_BODY_MANAGER_H_