#ifndef _EASYSPRPACKER_PACK_AUDIO_SPR_H_
#define _EASYSPRPACKER_PACK_AUDIO_SPR_H_

#include "PackNode.h"

#include <memory>

namespace eaudio { class Sprite; }

namespace esprpacker
{

class PackAudioSpr : public PackNode
{
public:
	PackAudioSpr(const std::shared_ptr<eaudio::Sprite>& spr);
	virtual ~PackAudioSpr();

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

	bool Equal(const std::shared_ptr<eaudio::Sprite>& spr) const { return true; }

private:
	const PackNode* m_sym;

}; // PackAudioSpr

}

#endif // _EASYSPRPACKER_PACK_AUDIO_SPR_H_