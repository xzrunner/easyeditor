#pragma once

#include "ISymbol.h"

#include "common/Rect.h"
#include "common/Scale9FileAdapter.h"

namespace d2d
{
	class Scale9Symbol : public ISymbol
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
		Scale9Symbol();
		virtual ~Scale9Symbol();

		//
		// ICloneable interface
		//
		virtual Scale9Symbol* clone() const { return NULL; }

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
		virtual Rect getSize(const ISprite* sprite = NULL) const;

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

		static void initSprite(const Scale9FileAdapter::Entry& entry,
			ISprite** pSprite, const std::string& dlg);

	public:
		Type m_type;

		// 2
		// 1
		// 0 1 2
		ISprite* m_sprites[3][3];

		mutable float m_width, m_height;

	}; // Scale9Symbol
}

