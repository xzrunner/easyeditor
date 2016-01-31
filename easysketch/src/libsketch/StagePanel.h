#ifndef _LIBSKETCH_STAGE_PANEL_H_
#define _LIBSKETCH_STAGE_PANEL_H_


#include <easy3d.h>

namespace libsketch
{

class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
		ee::LibraryPanel* library);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;

	ivec2 TransPos3ProjectToScreen(const vec3& proj) const;
	vec3 TransPos3ScreenToProject(const ivec2& scr, float proj_z) const;

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
	std::vector<ee::Sprite*> m_sprites;

}; // StagePanel

}

#endif // _LIBSKETCH_STAGE_PANEL_H_
