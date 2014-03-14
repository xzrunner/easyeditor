#pragma once

#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Color.h"

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
		virtual ISymbol* clone() const { return NULL; }

		virtual void reloadTexture() const = 0;
		virtual void draw(const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const = 0;
		virtual Rect getSize(const ISprite* sprite = NULL) const = 0;

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

