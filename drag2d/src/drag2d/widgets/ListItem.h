#pragma once

#include "common/Object.h"
#include "interfaces.h"

#include <wx/wx.h>

namespace d2d
{
	class Bitmap;

	class ListItem : public Object, public ICloneable
	{
	public:
		ListItem() {
			m_bitmap = NULL;
		}
		virtual ~ListItem() {}

		virtual void refresh() {}

		const wxString& getName() const { return m_name; }
		void setName(const wxString& name) { m_name = name; }

		const wxString& getInfo() const { return m_info; }
		void setInfo(const wxString& info) { m_info = info; }

		const Bitmap* getBitmap() const { return m_bitmap; }

	protected:
		wxString m_name;
		wxString m_info;

		Bitmap* m_bitmap;

	}; // ListItem
}

