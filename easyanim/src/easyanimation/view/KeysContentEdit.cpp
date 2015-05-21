#include "KeysContentEdit.h"
#include "FramePropertySetting.h"

#include "dataset/LayersMgr.h"
#include "dataset/Layer.h"
#include "view/KeysPanel.h"
#include "view/StagePanel.h"
#include "frame/Controller.h"

namespace eanim
{

KeysContentEdit::KeysContentEdit(Controller* ctrl)
	: m_ctrl(ctrl)
{
	m_last_row = m_last_col = -1;
}

void KeysContentEdit::OnMouseLeftDown(int row, int col)
{
	LayersMgr& layers = m_ctrl->GetLayers();
	if (row >= layers.size()) {
		return;
	}

	m_last_col = col;
	m_last_row = row;
	if (wxGetKeyState(WXK_SHIFT)) {
		m_ctrl->GetKeysPanel()->SetSelectRegion(row, col);
	} else {
		m_ctrl->GetKeysPanel()->SetSelectPos(row, col);
	}

	bool selected = false;
	Layer* layer = layers.getLayer(layers.size() - row - 1);
	if (layer)
	{
		const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
		std::map<int, KeyFrame*>::const_iterator itr = frames.find(col+1);
		if (itr != frames.end())
		{
			selected = true;
			FramePropertySetting* property = 
				new FramePropertySetting(m_ctrl->GetStagePanel(), itr->second);
			m_ctrl->GetPropertyPanel()->setPropertySetting(property);
		}
	}
	if (!selected) {
		m_ctrl->GetPropertyPanel()->setPropertySetting(NULL);
	}
}

void KeysContentEdit::OnMouseLeftUp(int row, int col)
{
	m_last_row = m_last_col = -1;
}

void KeysContentEdit::OnMouseDragging(int row, int col)
{
	if (m_last_col == -1 || m_last_row == -1) {
		return;
	}

	if (row == m_last_row && row < m_ctrl->GetLayers().size()) {
		int col_min = std::min(m_last_col, col),
			col_max = std::max(m_last_col, col);
		m_ctrl->GetKeysPanel()->UpdateSelectRegion(col_min, col_max);
	}
}

void KeysContentEdit::CopySelection()
{
	int row, col_min, col_max;
	m_ctrl->GetKeysPanel()->GetSelectRegion(row, col_min, col_max);


// 	LayersMgr& layers = m_ctrl->GetLayers();
// 	int index = layers.size() - row - 1;
// 	Layer* layer = layers.getLayer(index);
// 	const std::map<int, KeyFrame*>& frames = layer->getAllFrames();
// 	std::map<int, KeyFrame*>::const_iterator itr_begin = frames.lower_bound(col_min + 1),
// 		itr_end = frames.upper_bound(col_max + 1);
// 	for (std::map<int, KeyFrame*>::const_iterator itr = itr_begin; itr != itr_end; ++itr) {
// 		m_selection.push_back(itr->second);
// 	}
}

void KeysContentEdit::PasteSelection()
{
}

void KeysContentEdit::DeleteSelection()
{
	int row, col_min, col_max;
	m_ctrl->GetKeysPanel()->GetSelectRegion(row, col_min, col_max);

	if (row == -1) {
		return;
	}

 	LayersMgr& layers = m_ctrl->GetLayers();
 	int index = layers.size() - row - 1;
 	Layer* layer = layers.getLayer(index);
	if (layer) {
		layer->RemoveFrameRegion(col_min + 1, col_max + 1);
	}
}

}