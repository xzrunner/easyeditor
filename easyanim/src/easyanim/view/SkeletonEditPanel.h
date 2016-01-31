#pragma once

#include "dataset/WholeSkeleton.h"



namespace eanim
{
	class Sprite;
	class SkeletonSymbolList;

	class SkeletonEditPanel : public ee::EditPanel, public ee::MultiSpritesImpl
	{
	public:
		static const wxString BONE_SYMBOL_LIST_NAME;

	public:
		SkeletonEditPanel(wxWindow* parent, WholeSkeleton* skeleton, 
			SkeletonSymbolList* symbolList, wxTextCtrl* layerTextCtrl);

		//
		// ee::MultiSpritesImpl interface
		//
		virtual void traverseSprites(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting,
			bool order = true) const;
		virtual void removeSprite(ee::ISprite* sprite);
		virtual void insertSprite(ee::ISprite* sprite) {}
		virtual void clearSprites() {}

		virtual void resetSpriteOrder(ee::ISprite* sprite, bool up) {}

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

