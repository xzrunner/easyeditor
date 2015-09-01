#ifndef _LIBCOCO_UNPACK_NODE_FACTORY_H_
#define _LIBCOCO_UNPACK_NODE_FACTORY_H_

// struct lua_State;
// 
// #include <map>
// #include <vector>

#include <drag2d.h>

namespace libcoco
{

class IPackNode;

class UnpackNodeFactory
{
public:
	void Unpack(lua_State* L, const std::vector<d2d::Image*>& images);

	IPackNode* Query(int id) const;
	IPackNode* Query(const std::string& name) const;

	void AddUnassigned(int id, const IPackNode** ret);

	void AfterUnpack();
	
	static UnpackNodeFactory* Instance();

private:
	UnpackNodeFactory();

private:
	std::map<int, IPackNode*> m_map_id;

	std::map<std::string, IPackNode*> m_map_name;

	mutable std::vector<std::pair<int, const IPackNode**> > m_unassigned;

private:
	static UnpackNodeFactory* m_instance;

}; // UnpackNodeFactory

}

#endif // _LIBCOCO_UNPACK_NODE_FACTORY_H_