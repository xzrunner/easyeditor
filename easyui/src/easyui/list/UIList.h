#ifndef _EASYUI_LIST_UI_LIST_H_
#define _EASYUI_LIST_UI_LIST_H_

#include <ee/Sprite.h>
#include <ee/Visitor.h>

#include <SM_Rect.h>

#include <json/json.h>

namespace eui
{
namespace list
{

class UIList
{
public:
	UIList();

	bool InsertSprite(ee::Sprite* sprite, int idx = -1);
	bool ClearAllSprite();
	void TraverseSprites(ee::Visitor<ee::Sprite>& visitor) const;

	void StoreToFile(const char* filename) const;
	void LoadFromFile(const char* filename);

	bool ReFilling();
	bool Arrange(const ee::Sprite* spr);

	sm::rect& GetClipbox() { return m_clipbox; }

	bool IsHoriEnable() const { return m_horizontal; }
	bool IsVertEnable() const { return m_vertical; }
	void EnableHori(bool enable);
	void EnableVert(bool enable);

	bool IsReverseOrderEnable() const { return m_reverse_order; }
	void EnableReverseOrder(bool enable) { m_reverse_order = enable; }

private:
	bool Filling();
	bool Arrange(float hori_space, float vert_space);

private:
	sm::rect m_clipbox;

	std::vector<ee::Sprite*> m_items;
//	ee::Sprite *m_base_spr, *m_hori_spr, *m_vert_spr;
	ee::Sprite* m_item_spr;

	bool m_horizontal;
	bool m_vertical;

	int m_hori_count;
	int m_vert_count;

	float m_hori_space;
	float m_vert_space;	

	bool m_reverse_order;

}; // UIList

}
}

#endif // _EASYUI_LIST_UI_LIST_H_