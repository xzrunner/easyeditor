#include "KeyDownHandler.h"

#include "dataset/DataMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/data_utility.h"
#include "view/StagePanel.h"
#include "message/messages.h"

#include <ee/panel_msg.h>

namespace eanim
{

KeyDownHandler* KeyDownHandler::m_instance = NULL;

KeyDownHandler* KeyDownHandler::Instance()
{
	if (!m_instance) {
		m_instance = new KeyDownHandler;
	}
	return m_instance;
}

bool KeyDownHandler::Process(int key_code)
{
	if (ee::GetKeyStateSJ::Instance()->Query(WXK_CONTROL) ||
		ee::GetKeyStateSJ::Instance()->Query(WXK_SHIFT)) {
		return false;
	}

	switch (key_code)
	{
	case 'z': case 'Z':
		SetPrevKeyFrame();
		return true;
	case 'x': case 'X':
		SetNextKeyFrame();
		return true;
	}
	return false;
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
		SetSelectedSJ::Instance()->Set(-1, next->GetTime() - 1);
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
		SetSelectedSJ::Instance()->Set(-1, next->GetTime() - 1);
	}
}

}