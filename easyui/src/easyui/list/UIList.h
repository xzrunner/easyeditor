#ifndef _EASYUI_LIST_UI_LIST_H_
#define _EASYUI_LIST_UI_LIST_H_

#include <drag2d.h>

#include <json/json.h>

namespace eui
{
namespace list
{

class UIList
{
public:
	UIList();

	bool InsertSprite(d2d::Sprite* sprite, int idx = -1);
	bool ClearAllSprite();
	void TraverseSprites(d2d::Visitor& visitor) const;

	void StoreToFile(const char* filename) const;
	void LoadFromFile(const char* filename);

	bool ReFilling();
	bool Arrange(const d2d::Sprite* spr);

	d2d::Rect& GetClipbox() { return m_clipbox; }

	bool IsHoriEnable() const { return m_horizontal; }
	bool IsVertEnable() const { return m_vertical; }
	void EnableHori(bool enable);
	void EnableVert(bool enable);

private:
	bool Filling();
	bool Arrange(float hori_space, float vert_space);

private:
	d2d::Rect m_clipbox;

	std::vector<d2d::Sprite*> m_items;
//	d2d::Sprite *m_base_spr, *m_hori_spr, *m_vert_spr;
	d2d::Sprite* m_item_spr;

	bool m_horizontal;
	bool m_vertical;

	int m_hori_count;
	int m_vert_count;

	float m_hori_space;
	float m_vert_space;	

}; // UIList

}
}

#endif // _EASYUI_LIST_UI_LIST_H_