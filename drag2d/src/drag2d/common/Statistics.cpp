#include "Statistics.h"

#include "dataset/SymbolMgr.h"
#include "dataset/Bitmap.h"
#include "dataset/Font.h"
#include "dataset/Image.h"

namespace d2d
{

void Statistics::PrintRes()
{
	wxString msg;
	msg += "Symbol: " + wxString::FromDouble(SymbolMgr::Instance()->size()) + "\n";
	msg += "Bitmap: " + wxString::FromDouble(BitmapMgr::Instance()->Size()) + "\n";
	msg += "Font: " + wxString::FromDouble(FontMgr::Instance()->Size()) + "\n";
	msg += "Image: " + wxString::FromDouble(ImageMgr::Instance()->Size()) + "\n";
	wxMessageBox(msg, wxT("Statistics"), wxOK | wxICON_INFORMATION/*, this*/);
}

}