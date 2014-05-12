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

		const Bitmap* getBitmap() const { return m_bitmap; }

	protected:
		wxString m_name;

		Bitmap* m_bitmap;

	}; // ListItem
}

