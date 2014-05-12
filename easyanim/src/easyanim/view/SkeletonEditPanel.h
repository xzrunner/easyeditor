#pragma once

#include "dataset/WholeSkeleton.h"

#include <drag2d.h>

namespace eanim
{
	class Sprite;
	class SkeletonSymbolList;

	class SkeletonEditPanel : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		static const wxString BONE_SYMBOL_LIST_NAME;

	public:
		SkeletonEditPanel(wxWindow* parent, WholeSkeleton* skeleton, 
			SkeletonSymbolList* symbolList, wxTextCtrl* layerTextCtrl);

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite) {}
		virtual void clearSprites() {}

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up) {}

		void insertSprite(int symbolIndex, wxCoord x, wxCoord y);

		void setSelectedBoneLayer(int layer);

	private:
 		class DragSymbolTarget : public wxTextDropTarget
 		{
 		public:
 			DragSymbolTarget(SkeletonEditPanel* panel);
 
 			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
 
 		private:
 			SkeletonEditPanel* m_panel;
 
 		}; // DragTileTarget

	private:
		WholeSkeleton* m_skeleton;
		SkeletonSymbolList* m_symbolList;

	}; // SkeletonEditPanel
}

