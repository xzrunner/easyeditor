#include "KeyDownHandler.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "view/StagePanel.h"
#include "message/GetCurrFrameSJ.h"
#include "message/SetCurrFrameSJ.h"

namespace eanim
{

KeyDownHandler::KeyDownHandler(LayersMgr* layers)
	: m_layers(layers)
{
}

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
	int layer_idx, frame_idx;
	GetCurrFrameSJ::Instance()->Get(layer_idx, frame_idx);
	Layer* layer = m_layers->GetLayer(layer_idx);
	if (!layer) {
		return;
	}
	KeyFrame* next = layer->GetPrevKeyFrame(frame_idx + 1);
	if (next) {
		SetCurrFrameSJ::Instance()->Set(-1, next->GetTime() - 1);
	}
}

void KeyDownHandler::SetNextKeyFrame()
{
	int layer_idx, frame_idx;
	GetCurrFrameSJ::Instance()->Get(layer_idx, frame_idx);
	Layer* layer = m_layers->GetLayer(layer_idx);
	if (!layer) {
		return;
	}
	KeyFrame* next = layer->GetNextKeyFrame(frame_idx + 1);
	if (next) {
		SetCurrFrameSJ::Instance()->Set(-1, next->GetTime() - 1);
	}
}

}