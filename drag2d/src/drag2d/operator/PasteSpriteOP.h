#pragma once

#include <vector>

#include "SelectSpritesOP.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{
	class ISprite;
	class MultiSpritesImpl;
	class PasteSpriteCMPT;

	class PasteSpriteOP : public SelectSpritesOP
	{
	public:
		PasteSpriteOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
			PropertySettingPanel* propertyPanel = NULL, PasteSpriteCMPT* cmpt = NULL);
		virtual ~PasteSpriteOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		void setMousePos(int x, int y);
		void fixPosOrthogonal();

	private:
		class SpriteBatch
		{
		public:
			SpriteBatch();
			~SpriteBatch();

			void loadFromSelection(const SpriteSelection& selection);

			void insertToSpritesImpl(MultiSpritesImpl* spritesImpl, const Vector& pos,
				bool isHorMirror, bool isVerMirror);
			void draw(const Vector& pos, bool isHorMirror, bool isVerMirror) const;

			const Vector& getCenter() const { return m_center; }

			void clear();

			bool empty() const { return m_selected.empty(); }

		private:
			void computeCenter();

		private:
			std::vector<ISprite*> m_selected;
			Vector m_center;

		}; // SpriteBatch

	private:
		MultiSpritesImpl* m_spritesImpl;

		PasteSpriteCMPT* m_cmpt;

		SpriteSelection* m_selection;

		Vector m_pos;

		SpriteBatch m_batch;

	}; // PasteSpriteOP
}

