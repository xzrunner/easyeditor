#ifndef _LIBRESPACKER_UNPACK_NODE_FACTORY_H_
#define _LIBRESPACKER_UNPACK_NODE_FACTORY_H_

// struct lua_State;
// 
// #include <map>
// #include <vector>



namespace librespacker
{

class IPackNode;

class UnpackNodeFactory
{
public:
	void UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images);
	const IPackNode* UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images,
		const std::map<int, std::string>& map_export);

	IPackNode* Query(int id) const;
	IPackNode* Query(const std::string& name) const;

	void AddUnassigned(int id, const IPackNode** ret);

	void AfterUnpack();
	
	static UnpackNodeFactory* Instance();

private:
	UnpackNodeFactory();

	void UpdateMapName(IPackNode* node);

private:
	std::map<int, IPackNode*> m_map_id;

	std::map<std::string, IPackNode*> m_map_name;

	mutable std::vector<std::pair<int, const IPackNode**> > m_unassigned;

private:
	static UnpackNodeFactory* m_instance;

}; // UnpackNodeFactory

}

#endif // _LIBRESPACKER_UNPACK_NODE_FACTORY_H_