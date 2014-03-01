
#ifndef EBUILDER_LAYERS_PAGE_H
#define EBUILDER_LAYERS_PAGE_H

#include <drag2d.h>

namespace ebuilder
{
	class LayersPage : public d2d::ILibraryPage
	{
	public:
		LayersPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

		void refreshThumbnail();

		void insert();

		void enableDelBtn();

		void onSelect(int index, int size);

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event);
		virtual void onDelPress(wxCommandEvent& event);

	private:
		void onUpPress(wxCommandEvent& event);
		void onDownPress(wxCommandEvent& event);

	private:
		wxButton *m_btnUp, *m_btnDown;

	}; // LayersPage
}

#endif // EBUILDER_LAYERS_PAGE_H
