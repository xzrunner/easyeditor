#pragma once

#include <vector>

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class ComplexSymbol : public ISymbol
	{
	public:
		ComplexSymbol();
		virtual ~ComplexSymbol();

		//
		// ICloneable interface
		//
		virtual ComplexSymbol* clone() const { return NULL; }

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

		bool isOneLayer() const;

	protected:
		virtual void loadResources();

	private:
		void initBounding();

		void refreshThumbnail();

		// todo!
	public:
		// avoid to cycle same ComplexSymbol
		void getAllChildren(std::vector<std::pair<const ISprite*, d2d::Vector> >& children) const;

	private:
		static const float SCALE;

	public:
		struct Group
		{
			std::string name;
			std::vector<ISprite*> members;
		};

	public:
		std::vector<ISprite*> m_sprites;

		std::vector<Group> m_groups;

		Rect m_rect;

		Rect m_clipbox;

	}; // ComplexSymbol
}

