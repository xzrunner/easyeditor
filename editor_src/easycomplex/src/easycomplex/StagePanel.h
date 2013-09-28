#pragma once

#include <drag2d.h>
#include <easycomplex.h>

namespace ecomplex
{
	class StagePanel : public libcomplex::StagePanel
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
			d2d::ComplexSymbol* symbol);

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(StagePanel* stage);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			StagePanel* m_stage;

		}; // DragTileTarget

	}; // StagePanel
}

