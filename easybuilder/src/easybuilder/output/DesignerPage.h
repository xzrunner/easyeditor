
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
			Canvas(DesignerPage* panel);

		protected:
			virtual void onDraw();

		private:
			void drawBackground();
			void drawCenter();

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

		//////////////////////////////////////////////////////////////////////////

		class TranslateActorState : public d2d::TranslateSpriteState
		{
		public:
			TranslateActorState(d2d::SpriteSelection* selection, 
				const d2d::Vector& first_pos)
				: d2d::TranslateSpriteState(selection, first_pos) {}

		protected:
			virtual void Translate(const d2d::Vector& offset);
		}; // TranslateActorState

		class RotateActorState : public d2d::RotateSpriteState
		{
		public:
			RotateActorState(d2d::SpriteSelection* selection, 
				const d2d::Vector& first_pos)
				: d2d::RotateSpriteState(selection, first_pos) {}

		protected:
			virtual void Rotate(const d2d::Vector& dst);
		}; // RotateActorState 

		class ScaleActorState : public d2d::ScaleSpriteState
		{
		public:
			ScaleActorState(d2d::ISprite* sprite, 
				const d2d::SpriteCtrlNode::Node& ctrl_node)
				: d2d::ScaleSpriteState(sprite, ctrl_node) {}

		protected:
			void Scale(const d2d::Vector& curr);
		}; // ScaleActorState 

		class ArrangeActorImpl : public d2d::ArrangeSpriteImpl
		{
		public:
			ArrangeActorImpl(DesignerPage* editPanel);

			virtual void onMouseLeftUp(int x, int y);
			virtual void onMouseRightUp(int x, int y);

		protected:
			virtual d2d::IArrangeSpriteState* CreateTransalteState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const;
			virtual d2d::IArrangeSpriteState* CreateRotateState(d2d::SpriteSelection* selection, const d2d::Vector& first_pos) const;
			virtual d2d::IArrangeSpriteState* CreateScaleState(d2d::ISprite* sprite, const d2d::SpriteCtrlNode::Node& ctrl_node) const;

		private:
			DesignerPage* m_editPanel;

		}; // ArrangeActorImpl

		class ArrangeActorOP : public d2d::ArrangeSpriteOP<SelectActorOP>
		{
		public:
			ArrangeActorOP(DesignerPage* editPanel, d2d::AbstractEditCMPT* callback);
		}; // ArrangeActorOP

		//////////////////////////////////////////////////////////////////////////

	}; // DesignerPage
}

#endif // EBUILDER_DESIGNER_PANEL_H
