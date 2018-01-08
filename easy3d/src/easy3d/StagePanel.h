#pragma once

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>

namespace ee { class LibraryPanel; }

namespace e3d
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