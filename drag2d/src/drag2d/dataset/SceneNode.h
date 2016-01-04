#ifndef _DRAG2D_SCENE_NODE_H_
#define _DRAG2D_SCENE_NODE_H_

#include <set>

namespace d2d
{

class SceneNode
{
public:
	virtual ~SceneNode() {}
	virtual void Update(float dt) = 0;
	virtual void Draw() const = 0;
}; // SceneNode

class SceneNodeMgr
{
public:
	void Add(SceneNode* node);
	void Remove(SceneNode* node);

	void Update(float dt);
	void Draw() const;

	static SceneNodeMgr* Instance();

private:
	SceneNodeMgr() {}

private:
	std::set<SceneNode*> m_nodes;

private:
	static SceneNodeMgr* m_instance;

}; // SceneNodeMgr

}

#endif // _DRAG2D_SCENE_NODE_H_
