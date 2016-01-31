
#ifndef EBUILDER_DESIGNER_PANEL_H
#define EBUILDER_DESIGNER_PANEL_H



namespace ebuilder
{
	class Scene;
	class SceneItem;
	class LayerItem;
	class Actor;
	class Layer;

	class DesignerPage : public ee::EditPanel, public ee::MultiSpritesImpl
	{
	public:
		DesignerPage(wxWindow* parent, wxTopLevelWindow* frame);

		//
		// ee::MultiSpritesImpl interface
		//
		virtual void traverseSprites(ee::IVisitor& visitor, 
			ee::TraverseType type = ee::e_allExisting, bool order = true) const;
		virtual void removeSprite(ee::ISprite* sprite);
		virtual void insertSprite(ee::ISprite* sprite);
		virtual void clearSprites();

		virtual void resetSpriteOrder(ee::ISprite* sprite, bool up);

		void updateCodePage();

	private:
		class Canvas : public ee::SpriteStageCanvas
		{
		public:
			Canvas(DesignerPage* panelscr);

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

		class SelectActorOP : public ee::SelectSpritesOP
		{
		public:
			SelectActorOP(ee::EditPanel* editPanel, 
				ee::MultiSpritesImpl* spritesImpl,
				ee::PropertySettingPanel* propertyPanel,
				ee::AbstractEditCMPT* callback = NULL);
			virtual ~SelectActorOP();

			virtual bool onKeyDown(int keyCode);
			virtual bool onMouseLeftUp(int x, int y);

			virtual bool clear();

			virtual ee::IPropertySetting* 
				createPropertySetting(ee::ISprite* sprite) const;

		private:
			void clearBackstage();
			void clearClipboard();

		private:
			std::vector<Actor*> m_clipboard;

		}; // SelectActorOP

		//////////////////////////////////////////////////////////////////////////

		class TranslateActorState : public ee::TranslateSpriteState
		{
		public:
			TranslateActorState(ee::SpriteSelection* selection, 
				const ee::Vector& first_pos)
				: ee::TranslateSpriteState(selection, first_pos) {}

		protected:
			virtual void Translate(const ee::Vector& offset);
		}; // TranslateActorState

		class RotateActorState : public ee::RotateSpriteState
		{
		public:
			RotateActorState(ee::SpriteSelection* selection, 
				const ee::Vector& first_pos)
				: ee::RotateSpriteState(selection, first_pos) {}

		protected:
			virtual void Rotate(const ee::Vector& dst);
		}; // RotateActorState 

		class ScaleActorState : public ee::ScaleSpriteState
		{
		public:
			ScaleActorState(ee::ISprite* sprite, 
				const ee::SpriteCtrlNode::Node& ctrl_node)
				: ee::ScaleSpriteState(sprite, ctrl_node) {}

		protected:
			void Scale(const ee::Vector& curr);
		}; // ScaleActorState 

		class ArrangeActorImpl : public ee::ArrangeSpriteImpl
		{
		public:
			ArrangeActorImpl(DesignerPage* editPanel);

			virtual void onMouseLeftUp(int x, int y);
			virtual void onMouseRightUp(int x, int y);

		protected:
			virtual ee::IArrangeSpriteState* CreateTransalteState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const;
			virtual ee::IArrangeSpriteState* CreateRotateState(ee::SpriteSelection* selection, const ee::Vector& first_pos) const;
			virtual ee::IArrangeSpriteState* CreateScaleState(ee::ISprite* sprite, const ee::SpriteCtrlNode::Node& ctrl_node) const;

		private:
			DesignerPage* m_editPanel;

		}; // ArrangeActorImpl

		class ArrangeActorOP : public ee::ArrangeSpriteOP<SelectActorOP>
		{
		public:
			ArrangeActorOP(DesignerPage* editPanel, ee::AbstractEditCMPT* callback);
		}; // ArrangeActorOP

		//////////////////////////////////////////////////////////////////////////

	}; // DesignerPage
}

#endif // EBUILDER_DESIGNER_PANEL_H
