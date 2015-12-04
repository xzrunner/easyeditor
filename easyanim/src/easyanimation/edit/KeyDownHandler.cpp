#include "KeyDownHandler.h"

#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"
#include "message/messages.h"

namespace eanim
{

void KeyDownHandler::Process(int key_code)
{
	if (d2d::GetKeyStateSJ::Instance()->Query(WXK_CONTROL) ||
		d2d::GetKeyStateSJ::Instance()->Query(WXK_SHIFT)) {
		return;
	}

	switch (key_code)
	{
	case 'z': case 'Z':
		SetPrevKeyFrame();
		break;
	case 'x': case 'X':
		SetNextKeyFrame();
		break;
	}
}

void KeyDownHandler::SetPrevKeyFrame()
{
	int col;
	Layer* layer = get_curr_layer(col);
	if (!layer) {
		return;
	}
	KeyFrame* next = layer->GetPrevKeyFrame(col + 1);
	if (next) {
		SetCurrFrameSJ::Instance()->Set(-1, next->GetTime() - 1);
	}
}

void KeyDownHandler::SetNextKeyFrame()
{
	int col;
	Layer* layer = get_curr_layer(col);
	if (!layer) {
		return;
	}
	KeyFrame* next = layer->GetNextKeyFrame(col + 1);
	if (next) {
		SetCurrFrameSJ::Instance()->Set(-1, next->GetTime() - 1);
	}
}

}