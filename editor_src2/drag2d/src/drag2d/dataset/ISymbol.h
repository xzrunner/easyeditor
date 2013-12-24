#pragma once

#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"

namespace d2d
{
	class ISprite;

	class ISymbol : public ListItem, public ISerializable
	{
	public:
		virtual ~ISymbol() {}

		//
		// IObject interface
		//	
		virtual ISymbol* clone() { return NULL; }

		virtual void reloadTexture() const = 0;
		virtual void draw(const ISprite* sprite = NULL) const = 0;
		virtual float getWidth(const ISprite* sprite = NULL) const = 0;
		virtual float getHeight(const ISprite* sprite = NULL) const = 0;

		bool loadFromFile(const wxString& filepath);

		const wxString& getFilepath() const { 
			return m_filepath; 
		}

	public:
		std::string name;

	protected:
		virtual void loadResources() = 0;

	protected:
		wxString m_filepath;

	}; // ISymbol
}

