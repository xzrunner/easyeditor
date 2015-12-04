#include "KeysContentEdit.h"
#include "KeyFramePropertySetting.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"
#include "dataset/DataMgr.h"
#include "view/KeysPanel.h"
#include "view/StagePanel.h"
#include "view/ViewMgr.h"
#include "message/messages.h"

#include <wx/clipbrd.h>

namespace eanim
{

KeysContentEdit::KeysContentEdit()
{
	m_last_row = m_last_col = -1;
}

void KeysContentEdit::OnMouseLeftDown(int row, int col)
{
	int layer_sz = DataMgr::Instance()->GetLayers().Size();
	if (row >= layer_sz) {
		return;
	}

	m_last_col = col;
	m_last_row = row;
	if (d2d::GetKeyStateSJ::Instance()->Query(WXK_SHIFT)) {
		SetSelectedRegionSJ::Instance()->Set(row, col);
	} else {
		SetCurrFrameSJ::Instance()->Set(layer_sz - row - 1, col);
	}

	bool selected = false;
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(layer_sz - row - 1);
	if (layer)
	{
		const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
		std::map<int, KeyFrame*>::const_iterator itr = frames.find(col+1);
		if (itr != frames.end())
		{
			selected = true;
			KeyFramePropertySetting* property = 
				new KeyFramePropertySetting(itr->second);
			d2d::SetPropertySettingSJ::Instance()->Set(property);
		}
	}
	if (!selected) {
		d2d::SetPropertySettingSJ::Instance()->Set(NULL);
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

	if (row == m_last_row && row < DataMgr::Instance()->GetLayers().Size()) {
		int col_min = std::min(m_last_col, col),
			col_max = std::max(m_last_col, col);
		UpdateSelectedRegionSJ::Instance()->Update(col_min, col_max);
	}
}

void KeysContentEdit::CopySelection()
{
	int row, col_min, col_max;
	ViewMgr::Instance()->keys->GetSelectRegion(row, col_min, col_max);

	if (row == -1) {
		return;
	}

	Json::Value value;

	int index = DataMgr::Instance()->GetLayers().Size() - row - 1;
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
 	const std::map<int, KeyFrame*>& frames = layer->GetAllFrames();
 	std::map<int, KeyFrame*>::const_iterator itr_begin = frames.lower_bound(col_min + 1),
 		itr_end = frames.upper_bound(col_max + 1);
	int last_frame = col_min + 1;
 	for (std::map<int, KeyFrame*>::const_iterator itr = itr_begin; itr != itr_end; ++itr) {		
		Json::Value k_val;
		k_val["distance"] = itr->first - last_frame;

		const std::vector<d2d::ISprite*>& sprites = itr->second->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			d2d::ISprite* spr = sprites[i];
			Json::Value s_val;
			s_val["filename"] = spr->GetSymbol().GetFilepath();
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

	int row, col;
	ViewMgr::Instance()->keys->GetSelectPos(row, col);
	if (row == -1 || col == -1) {
		wxTheClipboard->Close();
		return;
	}

	size_t index = DataMgr::Instance()->GetLayers().Size() - row - 1;
	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
	for (int iframe = 0, n = value["frame"].size(); iframe < n; ++iframe) 
	{
		const Json::Value& k_val = value["frame"][iframe];

		int dis = k_val["distance"].asInt();
		for (int i = 0; i < dis; ++i) {
			layer->InsertNullFrame(col+1);
		}
		col += dis;

		KeyFrame* frame = new KeyFrame(col + 1);
		++col;

		for (int i_spr = 0, m = k_val["sprite"].size(); i_spr < m; ++i_spr) 
		{
			const Json::Value& s_val = k_val["sprite"][i_spr];
			
			std::string filepath = s_val["filename"].asString();
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			symbol->RefreshThumbnail(filepath);
			d2d::ISprite* spr = d2d::SpriteFactory::Instance()->create(symbol);
			spr->Load(s_val);
			frame->Insert(spr);
			spr->Release();
			symbol->Release();
		}

		layer->InsertKeyFrame(frame);
	}

	d2d::RefreshPanelSJ::Instance()->Refresh();

	//////////////////////////////////////////////////////////////////////////

	wxTheClipboard->Close();
}

void KeysContentEdit::DeleteSelection()
{
	int row, col_min, col_max;
	ViewMgr::Instance()->keys->GetSelectRegion(row, col_min, col_max);

	if (row == -1) {
		return;
	}

 	int index = DataMgr::Instance()->GetLayers().Size() - row - 1;
 	Layer* layer = DataMgr::Instance()->GetLayers().GetLayer(index);
	if (layer) {
		d2d::AbstractAtomicOP* aop = layer->RemoveFrameRegion(col_min + 1, col_max + 1);
		d2d::EditAddRecordSJ::Instance()->Add(aop);
	}
}

}