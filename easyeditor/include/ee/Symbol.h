#ifndef _EASYEDITOR_SYMBOL_H_
#define _EASYEDITOR_SYMBOL_H_

#include "ListItem.h"
#include "UserDataImpl.h"
#include "ResourcesMgr.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>

#include <set>

namespace s2 { class Symbol; class RenderParams; }

namespace ee
{

class Sprite;

class Symbol : public ListItem, public UserDataImpl
{
public:
	Symbol();
	Symbol(const Symbol& sym);
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const { return NULL; }

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual void Draw(const s2::RenderParams& params, const Sprite* spr = NULL) const = 0;
	virtual void ReloadTexture() const {}
	virtual sm::rect GetSize(const Sprite* sprite = NULL) const = 0;
	virtual void InvalidRect(const sm::mat4& mt) const {}

	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { 
		return m_filepath; 
	}
	void SetFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

	const std::set<std::string>& GetFilepaths() const;
	void SetFilepaths(const std::set<std::string>& filepaths);

private:
	const Symbol& operator = (const Symbol& sym) { return sym; }

public:
	std::string name;
	std::string tag;

protected:
	virtual void LoadResources() = 0;

protected:
	s2::Symbol* m_impl;

	std::string m_filepath;

private:
	std::set<std::string> m_filepaths;

}; // Symbol

class SymbolCmp
{
public:
	bool operator () (const Symbol* s0, const Symbol* s1) const {
		return s0->GetFilepath().compare(s1->GetFilepath()) < 0;
	}
}; // SymbolCmp

}

#endif // _EASYEDITOR_SYMBOL_H_