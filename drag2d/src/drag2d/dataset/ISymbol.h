#ifndef _DRAG2D_SYMBOL_H_
#define _DRAG2D_SYMBOL_H_

#include "ColorTrans.h"
#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"
#include "common/Rect.h"
#include "common/Color.h"
#include "common/UserDataImpl.h"

#include <set>

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
	virtual ISymbol* Clone() const { return NULL; }

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual void ReloadTexture() const = 0;
	virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
		const ISprite* spr = NULL, const ISprite* root = NULL) const = 0;
	virtual Rect GetSize(const ISprite* sprite = NULL) const = 0;
	virtual void InvalidRect(const Matrix& mt) const {}

	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { 
		return m_filepath; 
	}
	void SetFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

	const std::set<std::string>& GetFilepaths() const;
	void SetFilepaths(const std::set<std::string>& filepaths);

public:
	std::string name;
	std::string tag;

protected:
	virtual void LoadResources() = 0;

protected:
	std::string m_filepath;

private:
	std::set<std::string> m_filepaths;

}; // ISymbol

class SymbolCmp
{
public:
	bool operator () (const d2d::ISymbol* s0, const d2d::ISymbol* s1) const {
		return s0->GetFilepath().compare(s1->GetFilepath()) < 0;
	}
}; // SymbolCmp

}

#endif // _DRAG2D_SYMBOL_H_