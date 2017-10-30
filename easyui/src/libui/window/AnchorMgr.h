#ifndef _EASYUI_WINDOW_ANCHOR_MGR_H_
#define _EASYUI_WINDOW_ANCHOR_MGR_H_

#include <ee/Visitor.h>
#include <ee/Sprite.h>

#include <SM_Vector.h>

#include <json/json.h>

namespace s2 { class RenderParams; class UpdateParams; }

namespace eui
{
namespace window
{

class AnchorMgr
{
public:
	AnchorMgr();
	~AnchorMgr();

	virtual bool Update(const s2::UpdateParams& up);

	void LoadFromFile(const Json::Value& value, 
		const std::vector<ee::SprPtr>& sprs);
	void StoreToFile(Json::Value& value, const std::string& dir) const;

	void OnViewChanged(int width, int height);
	void OnSprPosChanged(const ee::SprPtr& spr);

	void DrawSprites(const s2::RenderParams& params) const;
	void DrawNodes(const s2::RenderParams& params) const;

	void Traverse(ee::RefVisitor<ee::Sprite>& visitor);
	void Remove(const ee::SprPtr& spr);
	void Insert(const ee::SprPtr& spr);
	void Clear();
	void ResetOrder(const ee::SprPtr& spr, bool up);
	void ResetOrderMost(const ee::SprPtr& spr, bool up);

	static int GetAnchorRadius() { return RADIUS; }

private:
	static const int RADIUS = 10;

	static const int ANCHOR_COUNT = 9;

private:
	struct Anchor
	{
		sm::vec2 pos;
		std::vector<ee::SprPtr> sprs;
	}; // Anchor

private:
	Anchor* GetNearestAnchor(const sm::vec2& pos);
	void Move(const ee::SprPtr& spr);

	static void ChangeAnchorPos(Anchor& anchor, const sm::vec2& pos);

	static void LoadAnchorData(const std::vector<ee::SprPtr>& sprs,
		const Json::Value& value, Anchor& anchor);

private:
	// 0 1 2
	// 3 4 5
	// 6 7 8
	Anchor m_anchors[ANCHOR_COUNT];

	std::vector<ee::SprPtr> m_sprs;

}; // AnchorMgr

}
}

#endif // _EASYUI_WINDOW_ANCHOR_MGR_H_