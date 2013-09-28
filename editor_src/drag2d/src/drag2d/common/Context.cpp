#include "Context.h"

#include <wx/stdpaths.h>

namespace d2d
{

Context* Context::m_instance = NULL;

Context::Context() 
{
//	exePath = wxStandardPaths::Get().GetExecutablePath();

//	exePath = wxStandardPaths::Get().GetPluginsDir();

}

wxString Context::getDir()
{
	if (!resPath.empty())
		return resPath;

	return exePath;
}

Context* Context::Instance()
{
	if (!m_instance)
	{
		m_instance = new Context();
	}
	return m_instance;
}

} // d2d