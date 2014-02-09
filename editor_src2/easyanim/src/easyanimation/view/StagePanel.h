#pragma once

#include <wx/wx.h>
#include <wx/dnd.h>

#include <drag2d.h>

#include "SkeletonData.h"

namespace eanim
{
	class KeyFrame;

	class StagePanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		StagePanel(wxWindow* parent, wxTopLevelWindow* frame);
		virtual ~StagePanel();

		//
		// d2d::EditPanel interface
		//
		virtual void clear();

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		SkeletonData& getSkeletonData() { return m_skeletonData; }

	private:
		void onMenuAddJointNode(wxCommandEvent& event);

	private:
		class DragSymbolTarget : public wxTextDropTarget
		{
		public:
			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
		}; // DragSymbolTarget

	public:
		enum
		{
			Menu_AddJointNode = EditPanel::Menu_End,
			Menu_End
		};

		static std::string menu_entries[];

	private:
		SkeletonData m_skeletonData;

		friend class StageCanvas;

	}; // StagePanel
}

