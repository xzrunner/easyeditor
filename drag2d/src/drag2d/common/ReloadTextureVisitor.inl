#ifndef _DRAG2D_RELOAD_TEXTURE_VISITOR_INL_
#define _DRAG2D_RELOAD_TEXTURE_VISITOR_INL_

namespace d2d
{

template <typename T>
ReloadTextureVisitor<T>::ReloadTextureVisitor()
{
	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->ReloadBegin();
	}
}

template <typename T>
ReloadTextureVisitor<T>::~ReloadTextureVisitor()
{
	if (Config::Instance()->IsUseDTex()) {
		DrawCallBatching::Instance()->ReloadEnd();
	}
}

template <typename T>
void ReloadTextureVisitor<T>::Visit(Object* object, bool& next)
{
	T* item = dynamic_cast<T*>(object);
	if (item) {
		item->ReloadTexture();
	}
	next = true;
}

}

#endif // _DRAG2D_RELOAD_TEXTURE_VISITOR_INL_