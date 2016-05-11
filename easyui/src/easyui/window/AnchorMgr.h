#ifndef _EASYUI_WINDOW_ANCHOR_MGR_H_
#define _EASYUI_WINDOW_ANCHOR_MGR_H_

#include <ee/Observer.h>

#include <SM_Vector.h>

#include <json/json.h>

namespace ee { class Sprite; }

namespace eui
{
namespace window
{

class AnchorMgr : public ee::Observer
{
public:
	AnchorMgr();
	~AnchorMgr();

	void OnViewChanged(int width, int height);

	void OnSprPosChanged(ee::Sprite* spr);

	void Draw() const;

	void LoadFromFile(const Json::Value& value, 
		const std::vector<ee::Sprite*>& sprites);
	void StoreToFile(Json::Value& value) const;

	static int GetAnchorRadius() { return RADIUS; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Insert(ee::Sprite* spr);
	void Remove(ee::Sprite* spr);
	void Clear();

private:
	static const int RADIUS = 50;

	static const int ANCHOR_COUNT = 9;

private:
	struct Anchor
	{
		sm::vec2 pos;
		std::vector<ee::Sprite*> sprites;
		
	}; // Anchor

private:
	static void ChangeAnchorPos(Anchor& anchor, const sm::vec2& pos);

	static void LoadAnchorData(const std::vector<ee::Sprite*>& sprites,
		const Json::Value& value, Anchor& anchor);

private:
	// 0 1 2
	// 3 4 5
	// 6 7 8
	Anchor m_anchors[ANCHOR_COUNT];

}; // AnchorMgr

}
}

#endif // _EASYUI_WINDOW_ANCHOR_MGR_H_