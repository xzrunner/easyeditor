#ifndef _EASYSPRPACKER_PACK_AUDIO_SPR_H_
#define _EASYSPRPACKER_PACK_AUDIO_SPR_H_

#include "PackNode.h"

namespace eaudio { class Sprite; }

namespace esprpacker
{

class PackAudioSpr : public PackNode
{
public:
	PackAudioSpr(const eaudio::Sprite* spr);
	virtual ~PackAudioSpr();

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

	bool Equal(const eaudio::Sprite* spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackAudioSpr

}

#endif // _EASYSPRPACKER_PACK_AUDIO_SPR_H_