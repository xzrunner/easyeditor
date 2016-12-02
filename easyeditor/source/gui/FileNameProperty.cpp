#include "FileNameProperty.h"
#include "FileHelper.h"
#include "SymbolFile.h"
#include "SymbolType.h"

#include <sprite2/SymType.h>

namespace ee
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(FileNameProperty, wxLongStringProperty, wxSize, const wxSize&, TextCtrlAndButton)

FileNameProperty::FileNameProperty(const wxString& label, const wxString& name, const wxString& value)
	: wxLongStringProperty(label, name, FileHelper::GetFilename(value.ToStdString()))
	, m_filepath(value)
{
}

bool FileNameProperty::OnButtonClick(wxPropertyGrid* propgrid, wxString& value)
{
	int type = SymbolFile::Instance()->Type(m_filepath.ToStdString());
	switch (type)
	{
	case s2::SYM_COMPLEX:
		wxExecute("easycomplex.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_ANIMATION:
		wxExecute("easyanimation.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_ICON:
		wxExecute("easyicon.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_MASK:
		wxExecute("easymask.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_MESH:
		wxExecute("easymesh.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_PARTICLE3D:
		wxExecute("easyparticle3d.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_PARTICLE2D:
		wxExecute("easyparticle2d.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_SCALE9:
		wxExecute("easyscale9.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_TEXTURE:
		wxExecute("easytexture.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case s2::SYM_TRAIL:
		wxExecute("easytrail.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	case ee::SYM_UI: case ee::SYM_UIWND:
		wxExecute("easyui.exe " + m_filepath, wxEXEC_ASYNC, NULL);
		break;
	}

	return false;
}

void FileNameProperty::SetFilepath(const std::string& filepath)
{
	m_filepath = filepath;
	SetValue(FileHelper::GetFilename(filepath));
}

}