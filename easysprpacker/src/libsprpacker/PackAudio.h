#ifndef _EASYSPRPACKER_PACK_AUDIO_H_
#define _EASYSPRPACKER_PACK_AUDIO_H_

#include "PackNode.h"

#include <sprite2/AudioSymbol.h>

namespace eaudio { class Symbol; }

namespace esprpacker
{

class PackAudio : public PackNode
{
public:
	PackAudio(const eaudio::Symbol* sym);

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
	
private:
	void Init(const eaudio::Symbol* sym);	

private:
	std::string m_audio_name;
	int m_audio_id;

}; // PackAudio

}

#endif // _EASYSPRPACKER_PACK_AUDIO_H_