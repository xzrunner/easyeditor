#ifndef _EASYEDITOR_SYMBOL_H_
#define _EASYEDITOR_SYMBOL_H_

#include "ListItem.h"
#include "UserDataImpl.h"
#include "ResourcesMgr.h"

#include <SM_Matrix.h>
#include <SM_Rect.h>
#include <sprite2/S2_Symbol.h>

#include <set>

namespace ee
{

class Sprite;

class Symbol : public virtual s2::Symbol, public ListItem, public UserDataImpl
{
public:
	virtual ~Symbol();

	/**
	 *  @interface
	 *    UserDataImpl
	 */
	virtual void ClearUserData(bool deletePtr);

	virtual void ReloadTexture() const {}
	virtual void InvalidRect(const sm::mat4& mt) const {}
	virtual void Traverse(Visitor<Sprite>& visitor) {}

	bool LoadFromFile(const std::string& filepath);

	const std::string& GetFilepath() const { 
		return m_filepath; 
	}
	void SetFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

	const std::set<std::string>& GetFilepaths() const;
	void SetFilepaths(const std::set<std::string>& filepaths);

	bool IsEditDirty() const { return m_edit_dirty; }
	void SetEditDirty(bool dirty) { m_edit_dirty = dirty; }

public:
	std::string name;
	std::string tag;

protected:
	virtual bool LoadResources() = 0;

protected:
	std::string m_filepath;

private:
	std::set<std::string> m_filepaths;

	bool m_edit_dirty;

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