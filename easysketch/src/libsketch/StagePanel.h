#ifndef _EASYSKETCH_STAGE_PANEL_H_
#define _EASYSKETCH_STAGE_PANEL_H_

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>

#include <easy3d.h>

namespace ee { class LibraryPanel; }

namespace esketch
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

	ee::ivec2 TransPos3ProjectToScreen(const ee::vec3& proj) const;
	ee::vec3 TransPos3ScreenToProject(const ee::ivec2& scr, float proj_z) const;

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

#endif // _EASYSKETCH_STAGE_PANEL_H_
