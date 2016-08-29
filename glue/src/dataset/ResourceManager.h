#ifndef _GLUE_RESOURCE_MANAGER_H_
#define _GLUE_RESOURCE_MANAGER_H_

#include <map>

namespace glue
{

template<class T>
class ResourceManager
{
public:
	T* Create(const std::string& filepath);
	void Release(const std::string& filepath);

	void Clear();

	static ResourceManager* Instance();

private:
	ResourceManager();
	~ResourceManager();

private:
	std::map<std::string, T*> m_res_map;

	static ResourceManager<T>* m_instance;

}; // ResourceManager

}

#include "ResourceManager.inl"

#endif // _GLUE_RESOURCE_MANAGER_H_