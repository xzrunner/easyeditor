#pragma once

#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Color.h"
#include "common/UserDataImpl.h"

namespace d2d
{
	class ISprite;

	class ISymbol : public ListItem, public UserDataImpl
	{
	public:
		virtual ~ISymbol();

		//
		// IObject interface
		//	
		virtual ISymbol* clone() const { return NULL; }

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr);

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

	class SymbolCmp
	{
	public:
		bool operator () (const d2d::ISymbol* s0, const d2d::ISymbol* s1) const {
			return s0->getFilepath() < s1->getFilepath();
		}
	}; // SymbolCmp
}

