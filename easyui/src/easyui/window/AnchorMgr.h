#ifndef _EASYUI_WINDOW_ANCHOR_MGR_H_
#define _EASYUI_WINDOW_ANCHOR_MGR_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class AnchorMgr
{
public:
	AnchorMgr();
	~AnchorMgr();

	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);

	void OnViewChanged(int width, int height);

	void Draw() const;

	void LoadFromFile(const Json::Value& value, 
		const std::vector<d2d::ISprite*>& sprites);
	void StoreToFile(Json::Value& value) const;

	static int GetAnchorRadius() { return RADIUS; }

private:
	static const int RADIUS = 30;

private:
	struct Anchor
	{
		d2d::Vector pos;
		std::vector<d2d::ISprite*> sprites;
		
	}; // Anchor

private:
	static void ChangeAnchorPos(Anchor& anchor, const d2d::Vector& pos);

private:
	// 0 1 2
	// 3 4 5
	Anchor m_anchors[6];

}; // AnchorMgr

}
}

#endif // _EASYUI_WINDOW_ANCHOR_MGR_H_