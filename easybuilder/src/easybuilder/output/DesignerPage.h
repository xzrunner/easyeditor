
#ifndef EBUILDER_DESIGNER_PANEL_H
#define EBUILDER_DESIGNER_PANEL_H

#include <drag2d.h>

namespace ebuilder
{
	class Scene;
	class SceneItem;
	class LayerItem;
	class Actor;
	class Layer;

	class DesignerPage : public d2d::EditPanel, public d2d::MultiSpritesImpl
	{
	public:
		DesignerPage(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// d2d::MultiSpritesImpl interface
		//
		virtual void traverseSprites(d2d::IVisitor& visitor, 
			d2d::TraverseType type = d2d::e_allExisting, bool order = true) const;
		virtual void removeSprite(d2d::ISprite* sprite);
		virtual void insertSprite(d2d::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(d2d::ISprite* sprite, bool up);

		void updateCodePage();

	private:
		class Canvas : public d2d::SpriteStageCanvas
		{
		public:
			Canvas(DesignerPage* panelscr);

		protected:
			virtual void onDraw(const d2d::Screen& scr);

		private:
			void drawBackground(const d2d::Screen& scr);
			void drawCenter(const d2d::Screen& scr);

		}; // Canvas

		class DragActorTarget : public wxTextDropTarget
		{
		public:
			DragActorTarget(DesignerPage* editPanel);

			virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

		private:
			DesignerPage* m_editPanel;

		}; // DragActorTarget

		class SelectActorOP : public d2d::SelectSpritesOP
		{
		public:
			SelectActorOP(d2d::EditPanel* editPanel, 
				d2d::MultiSpritesImpl* spritesImpl,
				d2d::PropertySettingPanel* propertyPanel,
				d2d::AbstractEditCMPT* callback = NULL);
			virtual ~SelectActorOP();

			virtual bool onKeyDown(int keyCode);
			virtual bool onMouseLeftUp(int x, int y);

			virtual bool clear();

			virtual d2d::IPropertySetting* 
				createPropertySetting(d2d::ISprite* sprite) const;

		private:
			void clearBackstage();
			void clearClipboard();

		private:
			std::vector<Actor*> m_clipboard;

		}; // SelectActorOP

		class ArrangeActorOP : public d2d::ArrangeSpriteOP<SelectActorOP>
		{
		public:
			ArrangeActorOP(DesignerPage* editPanel,
				d2d::AbstractEditCMPT* callback);
			
			virtual bool onMouseLeftUp(int x, int y);
			virtual bool onMouseRightUp(int x, int y);

		protected:
			virtual void translateSprite(const d2d::Vector& delta);
			virtual void rotateSprite(const d2d::Vector& dst);
			virtual void scaleSprite(const d2d::Vector& currPos);

		private:
			void refreshThumbnail();

		}; // ArrangeActorOP

	}; // DesignerPage
}

#endif // EBUILDER_DESIGNER_PANEL_H
