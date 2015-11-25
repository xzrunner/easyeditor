#ifndef _DRAG2D_RELOAD_TEXTURE_VISITOR_H_
#define _DRAG2D_RELOAD_TEXTURE_VISITOR_H_

namespace d2d
{

template <typename T>
class ReloadTextureVisitor : public IVisitor
{
public:
	ReloadTextureVisitor();
	virtual ~ReloadTextureVisitor();

	virtual void Visit(Object* object, bool& bFetchNext);

}; // ReloadTextureVisitor

}

#include "ReloadTextureVisitor.inl"

#endif // _DRAG2D_RELOAD_TEXTURE_VISITOR_H_
