#include "BinaryEPE.h"
#include "Exception.h"
#include "LuaDataHelper.h"
#include "tools.h"
#include "Lzma.h"

#include "epe/PackPicture.h"
#include "epe/PackAnimation.h"
#include "epe/PackLabel.h"

#include <spritepack.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#include <iostream>

#define STRINGIFY(A)  #A
#include "trans_old_ejoy2d_data2.lua"

namespace epbin
{

BinaryEPE::BinaryEPE(const std::string& infile)
	: m_texture(0)
	, m_maxid(0)
{
	Load(infile);
}

BinaryEPE::~BinaryEPE()
{
}

void BinaryEPE::Pack(const std::string& outfile, bool compress)
{
	try {
		PackImpl(outfile, compress);
	} catch (Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void BinaryEPE::Load(const std::string& infile)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_settop(L,0);

	int err = luaL_dostring(L, trans_old_ejoy2d_data2);
	if (err) {
		const char *msg = lua_tostring(L,-1);
		std::cout << msg << std::endl;
	}
	lua_getglobal(L, "trans2table");
	lua_pushstring(L, infile.c_str());
	err = lua_pcall(L, 1, 2, 0);
	if (err) {
		const char *msg = lua_tostring(L,-1);
		std::cout << msg << std::endl;
	}

	if (lua_gettop(L) != 1 || !lua_istable(L, 1)) {
		return;
	}

	int ani_maxid = 0;

	int len = lua_rawlen(L, 1);
	for(int i = 1; i <= len; i++)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, 1);

		if (lua_istable(L, -1))
		{
			std::string type = LuaDataHelper::GetStringField(L, "type");

			uint32_t id = LuaDataHelper::GetIntField(L, "id");
			if (id > m_maxid) {
				m_maxid = id;
			}

			if (LuaDataHelper::HasField(L, "export")) {
				std::string export_name = LuaDataHelper::GetStringField(L, "export");
				std::map<std::string, int>::iterator itr = m_map_export.find(export_name);
				if (itr != m_map_export.end()) {
					throw Exception("Duplicate export name %s", export_name.c_str());
				} else {
					m_map_export.insert(std::make_pair(export_name, id));
				}
			}

			if (type == "picture") {
				IPackNode* n = new epe::PackPicture(L, id);
				if (n->GetMaxID() > m_texture) {
					m_texture = n->GetMaxID();
				}
				m_nodes.push_back(n);
			} else if (type == "animation") {
				IPackNode* n = new epe::PackAnimation(L, id);
				if (n->GetMaxID() > ani_maxid) {
					ani_maxid = n->GetMaxID();
				}
				m_nodes.push_back(n);
			} else if (type == "label") {
				m_nodes.push_back(new epe::PackLabel(L, id));
			}
		}
		lua_pop(L,1);
	}

	if (ani_maxid > m_maxid) {
		throw Exception("Invalid id in animation %d", ani_maxid);
	}
}

void BinaryEPE::PackImpl(const std::string& outfile, bool compress)
{
	uint16_t export_n = m_map_export.size();
	uint16_t maxid = m_maxid;
	uint16_t tex = m_texture + 1;

	uint32_t size = 0;
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		IPackNode* node = m_nodes[i];
		size += node->Size();
	}
	int align_n = (m_maxid + 1 + 3) & ~3;
	size += SIZEOF_PACK 
		+ align_n * sizeof(uint8_t)
		+ (m_maxid+1) * SIZEOF_POINTER
		+ tex * sizeof(int);

	Block nodes_data = PackNodes2Mem(outfile);
	
	Block all_data;
	all_data.len = sizeof(export_n) + sizeof(maxid) + sizeof(tex) + sizeof(size) 
		+ sizeof(nodes_data.len) + nodes_data.len;
	std::map<std::string, int>::iterator itr = m_map_export.begin();
	for ( ; itr != m_map_export.end(); ++itr) {
		String str(itr->first);
		all_data.len += str.Size();
	}
	all_data.data = (uint8_t*)malloc(all_data.len);

	uint8_t* ptr = all_data.data;
	pack2mem(export_n, &ptr);	
	pack2mem(maxid, &ptr);	
	pack2mem(tex, &ptr);	
	pack2mem(nodes_data.len, &ptr);	
	itr = m_map_export.begin();
	for ( ; itr != m_map_export.end(); ++itr) {
		String str(itr->first);
		uint16_t id = itr->second;
		pack2mem(id, &ptr);
		str.Store(&ptr);
	}
	memcpy(ptr, nodes_data.data, nodes_data.len);
	free(nodes_data.data);

 	std::locale::global(std::locale(""));
 	std::ofstream fout(outfile.c_str(), std::ios::binary);
 	std::locale::global(std::locale("C"));
	if (compress)
	{
		uint8_t* dst = NULL;
		size_t dst_sz;
		Lzma::Compress(&dst, &dst_sz, all_data.data, all_data.len);

		fout.write(reinterpret_cast<const char*>(&dst_sz), sizeof(uint32_t));
		fout.write(reinterpret_cast<const char*>(dst), dst_sz);
	}
	else
	{
		int sz = -(int)all_data.len;
		fout.write(reinterpret_cast<const char*>(&sz), sizeof(int32_t));
		fout.write(reinterpret_cast<const char*>(all_data.data), all_data.len);
	}
	free(all_data.data);
	fout.close();
}

BinaryEPE::Block BinaryEPE::PackNodes2Mem(const std::string& outfile) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));

	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		IPackNode* node = m_nodes[i];
		uint16_t id = node->GetID();
		pack2file(id, fout);
		uint8_t type = node->GetType();
		pack2file(type, fout);
		node->Store(fout);
	}

	fout.close();

	std::locale::global(std::locale(""));
	std::ifstream fin(outfile.c_str(), std::ios::binary);
	std::locale::global(std::locale("C"));
	fin.seekg(0, std::ios::end);
	std::streamsize size = fin.tellg();
	fin.seekg(0, std::ios::beg);

	Block b;
	b.len = size;
	b.data = (uint8_t*)malloc(size);
	fin.read(reinterpret_cast<char*>(b.data), size);
	return b;
}

}