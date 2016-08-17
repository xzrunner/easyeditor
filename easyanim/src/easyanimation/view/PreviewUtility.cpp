#include "PreviewUtility.h"

#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"

#include <ee/PlayControl.h>
#include <ee/Sprite.h>

#include <assert.h>

namespace eanim
{

void PreviewUtility::GetCurrSprites(const ee::PlayControl& ctrl, std::vector<ee::Sprite*>& sprs)
{
	int frame_idx = ctrl.Frame();
	for (size_t i = 0, n = DataMgr::Instance()->GetLayers().Size(); i < n; ++i)
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(i);

		KeyFrame *curr_f = layer->GetCurrKeyFrame(frame_idx),
			*next_f = layer->GetNextKeyFrame(frame_idx);
		if (!curr_f)
			continue;

		if (!curr_f) 
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprs.push_back(curr_f->GetSprite(i)->EEClone());
			}
		}
		else if (!curr_f->HasClassicTween() || !next_f)
		{
			for (int i = 0, n = curr_f->Size(); i < n; ++i) {
				sprs.push_back(curr_f->GetSprite(i)->EEClone());
			}
		}
		else
		{
			assert(frame_idx >= curr_f->GetTime() && frame_idx < next_f->GetTime());
			float process = (float) (frame_idx - curr_f->GetTime()) / (next_f->GetTime() - curr_f->GetTime());
			KeyFrame::GetTweenSprite(curr_f, next_f, sprs, process);
		}
	}
}

}