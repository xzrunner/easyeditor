#include "StageDropTarget.h"
#include "Frame.h"
#include "EditPanel.h"
#include "MultiSpritesImpl.h"
#include "LibraryPanel.h"

#include "common/scripts.h"
#include "dataset/ScriptsSymbol.h"
#include "dataset/SpriteFactory.h"

namespace d2d
{

StageDropTarget::StageDropTarget(Frame* frame, EditPanel* edit_panel,
								 MultiSpritesImpl* sprites_impl, LibraryPanel* library)
	: m_frame(frame)
	, m_edit_panel(edit_panel)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
	SetDataObject(new Data(*this));
}

wxDragResult StageDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	if ( !GetData() )
		return wxDragNone;

	Data* dobj = (Data*)m_dataObject;
	return dobj->OnData(x, y) ? def : wxDragNone;
}

void StageDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
	wxString sType = text.substr(0, text.find(","));
	wxString sIndex = text.substr(text.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	ISymbol* symbol = m_library->getSymbol(index);
	if (symbol)
	{
		Vector pos = m_edit_panel->transPosScreenToProject(x, y);
		bool handled = OnDropSymbol(symbol, pos);
		if (!handled) {
			ISprite* sprite = SpriteFactory::Instance()->create(symbol);
			sprite->translate(pos);
			m_sprites_impl->insertSprite(sprite);
		}

// 		if (ScriptsSymbol* scripts = dynamic_cast<ScriptsSymbol*>(symbol)) 
// 		{
// 			scripts_do_string(scripts->getContent().c_str());
// 			//			scripts_do_file(symbol->getFilepath().c_str());
// 		}
// 		else
		//{
		//	Vector pos = m_edit_panel->transPosScreenToProject(x, y);
		//	ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		//	sprite->translate(pos);
		//	m_sprites_impl->insertSprite(sprite);
		//}
	}
}

void StageDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
 	if (filenames.size() != 1) {
 		return;
 	}
 
 	wxString filename = filenames[0];
 	m_frame->openFile(filename);
}

//////////////////////////////////////////////////////////////////////////
// class StageDropTarget::Data
//////////////////////////////////////////////////////////////////////////

static const wxChar* FORMAT_ID = wxT("StageDropData");

StageDropTarget::Data::
Data(StageDropTarget& drop)
	: m_drop(drop)
	, m_data_object_last(NULL)
{
	Add(new wxTextDataObject);
	Add(new wxFileDataObject);
}

bool StageDropTarget::Data::
SetData(const wxDataFormat& format, size_t len, const void *buf)
{
	m_data_object_last = GetObject(format);

	wxCHECK_MSG(m_data_object_last, FALSE,
		wxT("unsupported format in wxURLDataObject"));

	return m_data_object_last->SetData(len, buf);
}

bool StageDropTarget::Data::
OnData(wxCoord x, wxCoord y)
{
	bool ret = false;

	wxDataFormat format = m_data_object_last->GetPreferredFormat();
	if (format == wxDF_UNICODETEXT)
	{
		wxTextDataObject* obj = static_cast<wxTextDataObject*>(m_data_object_last);
		m_drop.OnDropText(x, y, obj->GetText());
		ret = true;
	}
	else if (format == wxDF_FILENAME)
	{
		wxFileDataObject* obj = static_cast<wxFileDataObject*>(m_data_object_last);
		m_drop.OnDropFiles(x, y, obj->GetFilenames());
		ret = true;
	}

	return ret;
}

}