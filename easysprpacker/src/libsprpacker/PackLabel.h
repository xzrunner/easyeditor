#ifndef _EASYSPRPACKER_PACK_LABEL_H_
#define _EASYSPRPACKER_PACK_LABEL_H_

#include "PackNode.h"

#include <sprite2/Textbox.h>

#include <memory>

namespace etext { class Sprite; }

namespace esprpacker
{

class PackLabel : public PackNode
{
public:
	PackLabel(const std::shared_ptr<etext::Sprite>& spr);

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const;	

	const s2::Textbox& GetTextBox() const { return m_tb; }

	const std::string& GetText() const { return m_text; }
	const std::string& GetTid() const { return m_tid; }

private:
	void Init(const std::shared_ptr<etext::Sprite>& spr);

private:
	s2::Textbox m_tb;

	std::string m_text;
	std::string m_tid;

	bool m_export;

}; // PackLabel

}

#endif // _EASYSPRPACKER_PACK_LABEL_H_