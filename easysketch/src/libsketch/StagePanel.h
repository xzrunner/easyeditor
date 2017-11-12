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
	virtual void TraverseSprites(ee::RefVisitor<ee::Sprite>& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const override;

	sm::ivec2 TransPos3ProjectToScreen(const sm::vec3& proj) const;
	sm::vec3 TransPos3ScreenToProject(const sm::ivec2& scr, float proj_z) const;

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void Insert(const ee::SprPtr& spr);
	void Remove(const ee::SprPtr& spr);
	void Clear();

private:
	std::vector<ee::SprPtr> m_sprs;

}; // StagePanel

}

#endif // _EASYSKETCH_STAGE_PANEL_H_
