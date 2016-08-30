#ifndef _EASYSPRPACKER_PACK_LABEL_H_
#define _EASYSPRPACKER_PACK_LABEL_H_

#include "PackNode.h"

#include <sprite2/Textbox.h>

namespace etext { class Sprite; }

namespace esprpacker
{

class PackLabel : public PackNode
{
public:
	PackLabel(const etext::Sprite* spr);
	virtual ~PackLabel();

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp, float scale) const;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const;
	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
		float scale) const;	

	const s2::Textbox& GetTextBox() const { return m_tb; }

	const std::string& GetText() const { return m_text; }
	const std::string& GetTid() const { return m_tid; }

private:
	void Init(const etext::Sprite* spr);

private:
	s2::Textbox m_tb;

	std::string m_text;
	std::string m_tid;

}; // PackLabel

}

#endif // _EASYSPRPACKER_PACK_LABEL_H_