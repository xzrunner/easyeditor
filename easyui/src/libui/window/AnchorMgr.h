#ifndef _EASYUI_WINDOW_ANCHOR_MGR_H_
#define _EASYUI_WINDOW_ANCHOR_MGR_H_

#include <ee/Visitor.h>

#include <SM_Vector.h>

#include <json/json.h>

namespace ee { class Sprite; }
namespace s2 { class RenderParams; }

namespace eui
{
namespace window
{

class AnchorMgr
{
public:
	AnchorMgr();
	~AnchorMgr();

	void LoadFromFile(const Json::Value& value, 
		const std::vector<ee::Sprite*>& sprs);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

	void OnViewChanged(int width, int height);
	void OnSprPosChanged(ee::Sprite* spr);

	void DrawSprites(const s2::RenderParams& params) const;
	void DrawNodes(const s2::RenderParams& params) const;

	void Traverse(ee::Visitor<ee::Sprite>& visitor);
	void Remove(ee::Sprite* spr);
	void Insert(ee::Sprite* spr);
	void Clear();
	void ResetOrder(ee::Sprite* spr, bool up);
	void ResetOrderMost(ee::Sprite* spr, bool up);

	static int GetAnchorRadius() { return RADIUS; }

private:
	static const int RADIUS = 10;

	static const int ANCHOR_COUNT = 9;

private:
	struct Anchor
	{
		sm::vec2 pos;
		std::vector<ee::Sprite*> sprs;
	}; // Anchor

private:
	Anchor* GetNearestAnchor(const sm::vec2& pos);
	void Move(ee::Sprite* spr);

	static void ChangeAnchorPos(Anchor& anchor, const sm::vec2& pos);

	static void LoadAnchorData(const std::vector<ee::Sprite*>& sprs,
		const Json::Value& value, Anchor& anchor);

private:
	// 0 1 2
	// 3 4 5
	// 6 7 8
	Anchor m_anchors[ANCHOR_COUNT];

	std::vector<ee::Sprite*> m_sprs;

}; // AnchorMgr

}
}

#endif // _EASYUI_WINDOW_ANCHOR_MGR_H_