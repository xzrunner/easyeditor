#ifndef _EASYEDITOR_RESOURCES_MGR_H_
#define _EASYEDITOR_RESOURCES_MGR_H_

#include <map>

namespace ee
{

class Visitor;

template<class T>
class ResourcesMgr
{
public:
	static ResourcesMgr* Instance();

	T* GetItem(const std::string& filename);
	void GetItem(const std::string& filename, T** old);

	void RemoveItem(const std::string& filename);

	void Clear();

	void Traverse(Visitor& visitor) const;

	size_t Size() const {
		return m_items.size();
	}

protected:
	ResourcesMgr();
	~ResourcesMgr();

private:
	static ResourcesMgr<T>* m_instance;

	std::map<std::string, T*> m_items;

}; // ResourcesMgr

}

#include "ResourcesMgr.inl"

#endif // _EASYEDITOR_RESOURCES_MGR_H_