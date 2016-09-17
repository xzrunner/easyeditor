#ifndef _EASYSPRPACKER_PACK_ANIMATION_H_
#define _EASYSPRPACKER_PACK_ANIMATION_H_

#include "PackNode.h"
#include "PackTrans.h"

#include <sprite2/AnimSymbol.h>

#include <vector>

namespace eanim { class Symbol; }

namespace esprpacker
{

class PackAnimation : public PackNode
{
public:
	PackAnimation(const eanim::Symbol* sym);
	virtual ~PackAnimation();

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
	
private:
	void Init(const eanim::Symbol* sym);

private:
	class Frame : public cu::RefCountObj
	{
	public:
		Frame(const s2::AnimSymbol::Frame* frame);
		~Frame();

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;	

	public:
		int m_index;
		std::vector<const PackNode*> m_sprs;
		bool m_tween;

	}; // Frame

	class Layer : public cu::RefCountObj
	{
	public:
		Layer(const s2::AnimSymbol::Layer* layer);
		~Layer();

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;	

	public:
		std::vector<Frame*> m_frames;
	}; // Layer
	
private:
	std::vector<Layer*> m_layers;

}; // PackAnimation

}

#endif // _EASYSPRPACKER_PACK_ANIMATION_H_