#ifndef _EASYPSEUDO3D_ISCENE_H_
#define _EASYPSEUDO3D_ISCENE_H_

namespace epseudo3d
{

class IScene
{
public:
	virtual ~IScene() {}
	virtual void Store(const char* filename) const = 0;
	virtual void Load(const char* filename) = 0;
	virtual void Draw() const = 0;
	virtual void DebugDraw() const = 0;

}; // IScene

}

#endif // _EASYPSEUDO3D_ISCENE_H_