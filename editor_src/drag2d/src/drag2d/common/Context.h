#pragma once

#include <wx/wx.h>

namespace d2d
{
	class Context
	{
	public:
		// The executable's directory
		wxString exePath;

		// Resource's dialog, usually is open file's directory
		wxString resPath;

	public:
		wxString getDir();

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

