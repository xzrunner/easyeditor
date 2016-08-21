#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Scale9Symbol.h>

#include <json/json.h>

namespace escale9
{

class Symbol : public ee::Symbol, public s2::Scale9Symbol
{
public:
	Symbol();

//	void ResizeScale9(float width, float height);
//	void ComposeFromSprites(ee::Sprite* sprs[3][3], float width, float height);

//	const Scale9Data& GetScale9Data() const { return m_data; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	ee::Sprite* LoadSprite(const Json::Value& spr_val, const std::string& dir);

//private:
//	Scale9Data m_data;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_