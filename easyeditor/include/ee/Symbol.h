#ifndef _EASYEDITOR_SYMBOL_H_
#define _EASYEDITOR_SYMBOL_H_

#include "ListItem.h"
#include "UserDataImpl.h"
#include "ResourcesMgr.h"
#include "Rect.h"

#include <SM_Matrix.h>

#include <set>

namespace ee
{

class Sprite;
class RenderParams;

class Symbol : public ListItem, public UserDataImpl
{
public:
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const { return NULL; }

	//
	// UserDataImpl interface
	//	
	virtual void ClearUserData(bool deletePtr);

	virtual void ReloadTexture() const = 0;
	virtual void Draw(const RenderParams& trans, const Sprite* spr = NULL, 
		const Sprite* root = NULL) const = 0;
	virtual Rect GetSize(const Sprite* sprite = NULL) const = 0;
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

public:
	std::string name;
	std::string tag;

protected:
	virtual void LoadResources() = 0;

protected:
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