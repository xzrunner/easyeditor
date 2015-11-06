#ifndef _EASYUI_WINDOW_ANCHOR_MGR_H_
#define _EASYUI_WINDOW_ANCHOR_MGR_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class AnchorMgr : public d2d::Observer
{
public:
	AnchorMgr();
	~AnchorMgr();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void OnViewChanged(int width, int height);

	void OnSprPosChanged(d2d::ISprite* spr);

	void Draw() const;

	void LoadFromFile(const Json::Value& value, 
		const std::vector<d2d::ISprite*>& sprites);
	void StoreToFile(Json::Value& value) const;

	static int GetAnchorRadius() { return RADIUS; }

private:
	void Insert(d2d::ISprite* spr);
	void Remove(d2d::ISprite* spr);
	void Clear();

private:
	static const int RADIUS = 50;

	static const int ANCHOR_COUNT = 9;

private:
	struct Anchor
	{
		d2d::Vector pos;
		std::vector<d2d::ISprite*> sprites;
		
	}; // Anchor

private:
	static void ChangeAnchorPos(Anchor& anchor, const d2d::Vector& pos);

	static void LoadAnchorData(const std::vector<d2d::ISprite*>& sprites,
		const Json::Value& value, Anchor& anchor);

private:
	// 0 1 2
	// 3 4 5
	// 6 7 8
	Anchor m_anchors[ANCHOR_COUNT];

	std::vector<d2d::Subject*> m_subjects;

}; // AnchorMgr

}
}

#endif // _EASYUI_WINDOW_ANCHOR_MGR_H_