#pragma once

#include <drag2d.h>

namespace coceditor
{
	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		virtual ~StagePanel();

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::SpritesPanelImpl interface
		//
		virtual void insertSprite(d2d::ISprite* sprite);

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			DragSymbolTarget(StagePanel* stage);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			StagePanel* m_stage;

		}; // DragSymbolTarget

	}; // StagePanel
}

