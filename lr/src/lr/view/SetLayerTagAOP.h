#ifndef _LR_SET_LAYER_TAG_AOP_H_
#define _LR_SET_LAYER_TAG_AOP_H_

#include <ee/AtomicOP.h>

namespace lr
{

class SetLayerTagAOP : public ee::AtomicOP
{
public:
	enum Type
	{
		COVER = 0,
		TOP,
		CLEAR,
	};

public:
	SetLayerTagAOP(Type type);

	virtual void Undo();
	virtual void Redo();
	virtual void Copy(const std::vector<ee::Sprite*>& sprs);

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprs) const;

	static void Set(const std::vector<ee::Sprite*>& sprs, Type type);

private:
	Type m_type;

}; // SetLayerTagAOP

}

#endif // _LR_SET_LAYER_TAG_AOP_H_