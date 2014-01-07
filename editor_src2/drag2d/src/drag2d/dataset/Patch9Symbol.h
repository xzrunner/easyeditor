#pragma once

#include "ISymbol.h"

#include "common/Rect.h"
#include "common/Patch9FileAdapter.h"

namespace d2d
{
	class Patch9Symbol : public ISymbol
	{
	public:
		enum Type
		{
			e_null = 0,
			e_9Grid,
			e_3GridHor,
			e_3GridVer,
			e_6GridUpper,
			e_9GridHollow
		};

	public:
		Patch9Symbol();
		virtual ~Patch9Symbol();

		//
		// ICloneable interface
		//
		virtual Patch9Symbol* clone() const { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		void composeFromSprites(ISprite* sprites[3][3], 
			float width, float height);

		void resize(float width, float height) const;

		Type type() const { return m_type; }

	protected:
		virtual void loadResources();

	private:
		void refreshThumbnail();

		void composeFromSprites() const;

		Type getType(ISprite* sprites[3][3]) const;

		static void stretch(ISprite* sprite, const d2d::Vector& center, 
			float width, float height);

		static void initSprite(const Patch9FileAdapter::Entry& entry,
			ISprite** pSprite, const std::string& dlg);

	public:
		Type m_type;

		// 2
		// 1
		// 0 1 2
		ISprite* m_sprites[3][3];

		mutable float m_width, m_height;

	}; // Patch9Symbol
}

