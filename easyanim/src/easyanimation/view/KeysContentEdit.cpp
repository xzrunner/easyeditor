#include "KeysContentEdit.h"
#include "FrameLerpDialog.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "view/KeysPanel.h"
#include "view/StagePanel.h"
#include "view/ViewMgr.h"
#include "message/messages.h"

#include <ee/panel_msg.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SymbolMgr.h>
#include <ee/SpriteFactory.h>

#include <wx/clipbrd.h>

#include <sstream>

namespace eanim
{

KeysContentEdit::KeysContentEdit(wxWindow* parent)
	: m_parent(parent)
{
	m_row = m_col = -1;
	m_col_min = m_col_max = -1;

	RegistSubject(SetSelectedSJ::Instance());
}

void KeysContentEdit::OnMouseLeftDown(int row, int col)
{
	int layer_sz = DataMgr::Instance()->GetLayers().Size();
	if (row < 0 || row >= layer_sz) {
		return;
	}

	int layer_idx = layer_sz - row - 1;
	if (ee::GetKeyStateSJ::Instance()->Query(WXK_SHIFT)) {
		UpdateRegion(layer_idx, col);
		SetSelectedRegionSJ::Instance()->Set(m_col);
	} else {
		m_row = row;
		m_col = col;
		m_col_min = m_col_max = col;
		SetSelectedSJ::Instance()->Set(layer_idx, col);
	}
}

void KeysContentEdit::OnMouseDragging(int row, int col)
{
	UpdateRegion(DataMgr::Instance()->GetLayers().Size() - 1 - row, col);
	SetSelectedRegionSJ::Instance()->Set(col);
}

void KeysContentEdit::OnMouseLeftClick(int row, int col)
{
	int layer_sz = DataMgr::Instance()->GetLayers().Size();
	int index = DataMgr::Instance()->GetLayers().Size() - row - 1;
	if (index < 0 || index >= layer_sz) {
		return;
	}
	
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
	KeyFrame* frame = layer->GetCurrKeyFrame(col + 1);
	if (frame && frame->HasClassicTween()) {
		FrameLerpDialog dlg(m_parent, frame);
		if (dlg.ShowModal() == wxID_OK) {
			dlg.Store();
		}
	}
}

void KeysContentEdit::CopySelection()
{
	if (m_row == -1 || m_col_min == -1 || m_col_max == -1) {
		return;
	}

	Json::Value value;

	int index = DataMgr::Instance()->GetLayers().Size() - m_row - 1;
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
 	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
 	std::map<int, KeyFrame*>::const_iterator itr_begin = frames.lower_bound(m_col_min + 1),
 		itr_end = frames.upper_bound(m_col_max + 1);
	int last_frame = m_col_min + 1;
 	for (std::map<int, KeyFrame*>::const_iterator itr = itr_begin; itr != itr_end; ++itr) {
		Json::Value k_val;
		k_val["distance"] = itr->first - last_frame;

		const std::vector<ee::SprPtr>& sprs = itr->second->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i) {
			auto& spr = sprs[i];
			Json::Value s_val;
			s_val["filepath"] = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
			spr->Store(s_val);	
			k_val["sprite"][i] = s_val;
		}

		last_frame = itr->first + 1;

		int sz = value["frame"].size();
		value["frame"][sz] = k_val;
 	}
	
	//////////////////////////////////////////////////////////////////////////

	Json::StyledStreamWriter writer;
	std::stringstream ss;
	writer.write(ss, value);
	wxTheClipboard->SetData(new wxTextDataObject(ss.str()));
	wxTheClipboard->Close();
}

void KeysContentEdit::PasteSelection()
{
	if (m_row == -1 || m_col == -1) {
		return;
	}

	if (!wxTheClipboard->Open()) {
		return;
	}

	if (!wxTheClipboard->IsSupported( wxDF_TEXT )) {
		wxTheClipboard->Close();
		return;
	}

	wxTextDataObject data;
	wxTheClipboard->GetData( data );

	Json::Value value;
	Json::Reader reader;
	std::string test = data.GetText().ToStdString();
	reader.parse(data.GetText().ToStdString(), value);

	if (value.isNull() || value["frame"].isNull()) {
		wxTheClipboard->Close();
		return;
	}

	//////////////////////////////////////////////////////////////////////////

	EnableObserve(false);

	size_t index = DataMgr::Instance()->GetLayers().Size() - m_row - 1;
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
	for (int iframe = 0, n = value["frame"].size(); iframe < n; ++iframe) 
	{
		const Json::Value& k_val = value["frame"][iframe];

		int dis = k_val["distance"].asInt();
		for (int i = 0; i < dis; ++i) {
			layer->InsertNullFrame(m_col+1);
		}
		m_col += dis;

		KeyFrame* frame = new KeyFrame(m_col + 1);
		layer->InsertKeyFrame(frame);

		++m_col;

		for (int i_spr = 0, m = k_val["sprite"].size(); i_spr < m; ++i_spr) 
		{
			const Json::Value& s_val = k_val["sprite"][i_spr];
			
			std::string filepath = s_val["filepath"].asString();
			auto sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			sym->RefreshThumbnail(filepath);
			auto spr = ee::SpriteFactory::Instance()->Create(sym);
			spr->Load(s_val);
			frame->Insert(spr, INT_MAX);
		}
	}

	ee::RefreshPanelSJ::Instance()->Refresh();

	EnableObserve(true);

	//////////////////////////////////////////////////////////////////////////

	wxTheClipboard->Close();
}

void KeysContentEdit::DeleteSelection()
{
	if (m_row == -1 || m_col_min == -1 || m_col_max == -1) {
		return;
	}

 	int index = DataMgr::Instance()->GetLayers().Size() - m_row - 1;
 	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
	if (layer) {
		auto aop = layer->RemoveFrameRegion(m_col_min + 1, m_col_max + 1);
		ee::EditAddRecordSJ::Instance()->Add(aop);
	}
}

void KeysContentEdit::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SET_CURR_FRAME:
		{
			SetSelectedSJ::Position* cf = (SetSelectedSJ::Position*)ud;
			if (cf->layer == -1 && cf->frame == -1) {
				m_row = m_col = -1;
				m_col_min = m_col_max = -1;
			} else {
				if (cf->layer != -1) {
					m_row = DataMgr::Instance()->GetLayers().Size() - 1 - cf->layer;
				}
				if (cf->frame != -1) {
					m_col = m_col_min = m_col_max = cf->frame;
				}
			}
		}
		break;
	}
}

void KeysContentEdit::UpdateRegion(int layer_idx, int frame_idx)
{
	if (layer_idx >= 0 && layer_idx < DataMgr::Instance()->GetLayers().Size()) 
	{
		Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(layer_idx);
		m_col_min = std::min(std::min(m_col, frame_idx), layer->GetMaxFrameTime() - 1);
		m_col_max = std::min(std::max(m_col, frame_idx), layer->GetMaxFrameTime() - 1);
	}
}

}