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

	void StoreToFile(const char* filename) const;
	void LoadFromFile(const char* filename);

	bool Filling();

	const d2d::Rect& GetClipbox() const { return m_clipbox; }
	void SetClipbox(const d2d::Rect& cb) { m_clipbox = cb; }

	d2d::SpritesContainer& GetItems() { return m_items; }
	const d2d::SpritesContainer& GetItems() const { return m_items; }

private:
	void HoriFilling(const std::vector<d2d::ISprite*>& ori_sprites, 
		std::vector<d2d::ISprite*>& new_sprites);
	void VertFilling(const std::vector<d2d::ISprite*>& ori_sprites, 
		std::vector<d2d::ISprite*>& new_sprites);

private:
	class ItemsCmp
	{
	public:
		bool operator() (const d2d::ISprite* item0, const d2d::ISprite* item1) const;
	}; // ItemsCmp

private:
	d2d::Rect m_clipbox;

	d2d::SpritesContainer m_items;

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