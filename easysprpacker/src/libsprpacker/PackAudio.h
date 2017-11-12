#ifndef _EASYSPRPACKER_PACK_AUDIO_H_
#define _EASYSPRPACKER_PACK_AUDIO_H_

#include "PackNode.h"

#include <sprite2/AudioSymbol.h>

#include <memory>

namespace eaudio { class Symbol; }

namespace esprpacker
{

class PackAudio : public PackNode
{
public:
	PackAudio(const std::shared_ptr<eaudio::Symbol>& sym);

	/**
	 *  @interface
	 *    lua
	 */
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const ee::TexturePacker& tp) const override;
	
	/**
	 *  @interface
	 *    bin
	 */
	virtual int SizeOfUnpackFromBin() const override;
	virtual int SizeOfPackToBin() const override;
	virtual void PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const override;	
	
private:
	void Init(const std::shared_ptr<eaudio::Symbol>& sym);

private:
	std::string m_audio_name;
	int m_audio_id;

}; // PackAudio

}

#endif // _EASYSPRPACKER_PACK_AUDIO_H_