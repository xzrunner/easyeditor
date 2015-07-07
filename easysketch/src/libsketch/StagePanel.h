#ifndef _LIBSKETCH_STAGE_PANEL_H_
#define _LIBSKETCH_STAGE_PANEL_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		d2d::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

	//
	// d2d::MultiSpritesImpl interface
	//
	virtual void traverseSprites(d2d::IVisitor& visitor, 
		d2d::DataTraverseType type = d2d::DT_ALL, bool order = true) const;
	virtual void removeSprite(d2d::ISprite* sprite);
	virtual void insertSprite(d2d::ISprite* sprite);
	virtual void clearSprites();
	virtual bool resetSpriteOrder(d2d::ISprite* sprite, bool up);

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;
	vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z) const;

private:
	std::vector<d2d::ISprite*> m_sprites;

}; // StagePanel

}

#endif // _LIBSKETCH_STAGE_PANEL_H_
