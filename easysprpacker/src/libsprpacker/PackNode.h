#ifndef _EASYSPRPACKER_PACK_NODE_H_
#define _EASYSPRPACKER_PACK_NODE_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

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
		const ee::TexturePacker& tp, float scale) const = 0;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const = 0;
	virtual int SizeOfPackToBin() const = 0;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const = 0;

	void SetName(const std::string& name) { m_name = name; }

	int GetID() const { return m_id; }

	void SetFilepath(const std::string& filepath) const {
		m_filepath = filepath;
	}
	const std::string& GetFilepath() const {
		return m_filepath;
	}

protected:
	std::string m_name;

	int m_id;

	mutable std::string m_filepath;

}; // PackNode

}

#endif // _EASYSPRPACKER_PACK_NODE_H_