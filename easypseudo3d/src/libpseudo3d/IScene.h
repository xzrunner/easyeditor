#ifndef _EASYPSEUDO3D_ISCENE_H_
#define _EASYPSEUDO3D_ISCENE_H_

namespace epseudo3d
{

class IScene
{
public:
	virtual ~IScene() {}
	virtual void Load() = 0;
	virtual void Draw() const = 0;

}; // IScene

}

#endif // _EASYPSEUDO3D_ISCENE_H_