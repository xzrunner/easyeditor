#pragma once

#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Color.h"
#include "common/UserDataImpl.h"

namespace d2d
{
	class ISprite;
	class Matrix;

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
		virtual void draw(const Matrix& mt,
			const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const Colorf& r_trans = Colorf(1, 0, 0, 0),
			const Colorf& g_trans = Colorf(0, 1, 0, 0),
			const Colorf& b_trans = Colorf(0, 0, 1, 0),
			const ISprite* sprite = NULL) const = 0;
		virtual Rect getSize(const ISprite* sprite = NULL) const = 0;

		bool loadFromFile(const wxString& filepath);

		const wxString& getFilepath() const { 
			return m_filepath; 
		}
		void SetFilepath(const wxString& filepath) {
			m_filepath = filepath;
		}

		const std::vector<std::string>& GetFilepaths() const;
		void SetFilepaths(const std::vector<std::string>& filepaths);

	public:
		std::string name;

	protected:
		virtual void loadResources() = 0;

	protected:
		wxString m_filepath;

	private:
		std::vector<std::string> m_filepaths;

	}; // ISymbol

	class SymbolCmp
	{
	public:
		bool operator () (const d2d::ISymbol* s0, const d2d::ISymbol* s1) const {
			return s0->getFilepath() < s1->getFilepath();
		}
	}; // SymbolCmp
}

