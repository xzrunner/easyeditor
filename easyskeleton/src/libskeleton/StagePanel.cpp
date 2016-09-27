#include "StagePanel.h"
#include "StageCanvas.h"
#include "ComposeSkeletonOP.h"
#include "Joint.h"
#include "Bone.h"

#include <ee/ArrangeSpriteOP.h>
#include <ee/SelectSpritesOP.h>
#include <ee/subject_id.h>
#include <ee/sprite_msg.h>

namespace eskeleton
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library,
					   ee::PropertySettingPanel* property)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
{
	ee::EditOP* op = new ComposeSkeletonOP(this, property);
	SetEditOP(op);
	op->RemoveReference();

	ee::StageCanvas* canvas = new StageCanvas(this);
	SetCanvas(canvas);
	canvas->RemoveReference();
}

StagePanel::~StagePanel()
{	
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	ee::SpritesPanelImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			ee::Sprite* spr = p->spr;
			if (!spr->GetUserData()) {
				spr->SetUserData(new Bone(spr));
			}
		}
		break;
	}
}

}