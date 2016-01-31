#pragma once



namespace coceditor
{
	class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		virtual ~StagePanel();

		//
		// ee::EditPanel interface
		//
		virtual void clear();

		//
		// ee::SpritesPanelImpl interface
		//
		virtual void insertSprite(ee::ISprite* sprite);

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

