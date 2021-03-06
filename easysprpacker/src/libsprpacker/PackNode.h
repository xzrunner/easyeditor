#ifndef _EASYSPRPACKER_PACK_NODE_H_
#define _EASYSPRPACKER_PACK_NODE_H_

#include <cu/CU_RefCountObj.h>
#include <cu/uncopyable.h>

#include <string>

#include <stdint.h>

namespace ebuilder { class CodeGenerator; }
namespace ee { class TexturePacker; class Image; }

namespace esprpacker
{

class PackNode : public cu::RefCountObj, private cu::Uncopyable
{
public:
	PackNode();
	virtual ~PackNode() {}

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const = 0;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const = 0;
	virtual int SizeOfPackToBin() const = 0;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const = 0;

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	uint32_t GetID() const;
	int GetPkgID() const { return m_pkg_id; }
	int GetNodeID() const { return m_node_id; }

	void SetFilepath(const std::string& filepath) const;
	const std::string& GetFilepath() const { return m_filepath; }

	void SetID(const std::string& filepath, bool force_curr = false) const;

private:
	std::string m_name;

	mutable int m_node_id;
	mutable int m_pkg_id;

	mutable std::string m_filepath;

}; // PackNode

}

#endif // _EASYSPRPACKER_PACK_NODE_H_