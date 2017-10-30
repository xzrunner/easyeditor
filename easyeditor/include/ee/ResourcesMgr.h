#ifndef _EASYEDITOR_RESOURCES_MGR_H_
#define _EASYEDITOR_RESOURCES_MGR_H_

#include "Visitor.h"

#include <map>

#include <string>

namespace ee
{

template<class T>
class ResourcesMgr
{
public:
	static ResourcesMgr* Instance();

	std::shared_ptr<T> GetItem(const std::string& filename);
	void RemoveItem(const std::string& filename);

	void Clear();

	void Traverse(RefVisitor<T>& visitor) const;

	size_t Size() const {
		return m_items.size();
	}

	void SetDefaultSym(const std::string& filepath) { m_default_sym = filepath; }
	const std::string& GetDefaultSym() const { return m_default_sym; }

protected:
	ResourcesMgr();
	~ResourcesMgr();

private:
	static ResourcesMgr<T>* m_instance;

	std::map<std::string, std::weak_ptr<T>> m_items;

	std::string m_default_sym;

}; // ResourcesMgr

}

#include "ResourcesMgr.inl"

#endif // _EASYEDITOR_RESOURCES_MGR_H_