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
		BG_DOWN,
		BG_UP,
		CLEAR,
	};

public:
	SetLayerTagAOP(Type type);

	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Copy(const std::vector<SprPtr>& sprs) override;

	virtual Json::Value Store(const std::vector<ee::SprPtr>& sprs) const override;

	static void Set(const std::vector<ee::SprPtr>& sprs, Type type);

private:
	Type m_type;

}; // SetLayerTagAOP

}

#endif // _LR_SET_LAYER_TAG_AOP_H_