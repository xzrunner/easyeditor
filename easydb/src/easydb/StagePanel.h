#ifndef EDB_STAGE_PANEL_H
#define EDB_STAGE_PANEL_H

#include <drag2d.h>

#include "Graphics.h"

namespace edb
{
	class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
		public d2d::ShapesPanelImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		virtual ~StagePanel();

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		void loadFromDir(const std::string& dirpath);
		void loadFromDirFast(const std::string& dirpath);

	private:
		void initConnection();

		void initPosition();

	private:
		Graphics m_graphics;

		friend class StageCanvas;

	}; // StagePanel
}

#endif // EDB_STAGE_PANEL_H