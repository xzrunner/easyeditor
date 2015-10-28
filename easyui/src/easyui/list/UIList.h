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

	void ClearExceptBase();

	bool ReorderSprite(d2d::ISprite* sprite, bool up);
	bool InsertSprite(d2d::ISprite* sprite, int idx = -1);
	bool RemoveSprite(d2d::ISprite* sprite);
	bool ClearAllSprite();
	void TraverseSprites(d2d::IVisitor& visitor) const;

	void StoreToFile(const char* filename) const;
	void LoadFromFile(const char* filename);

	bool Filling();

	const d2d::Rect& GetClipbox() const { return m_clipbox; }
	void SetClipbox(const d2d::Rect& cb) { m_clipbox = cb; }

	void ChangeHori();
	void ChangeVert();

private:
	d2d::Rect m_clipbox;

	d2d::SpritesContainer m_items;
	d2d::ISprite *m_base_spr, *m_hori_spr, *m_vert_spr;

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