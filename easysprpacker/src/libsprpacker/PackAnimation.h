#ifndef _EASYSPRPACKER_PACK_ANIMATION_H_
#define _EASYSPRPACKER_PACK_ANIMATION_H_

#include "PackNode.h"
#include "PackTrans.h"

#include <sprite2/AnimSymbol.h>

#include <vector>
#include <memory>

namespace libanim { class Symbol; }

namespace esprpacker
{

class PackAnimation : public PackNode
{
public:
	PackAnimation(const std::shared_ptr<libanim::Symbol>& sym);
	virtual ~PackAnimation();

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
	void Init(const std::shared_ptr<libanim::Symbol>& sym);

	void CheckLerp(const std::string& filepath);

private:
	class Actor : public cu::RefCountObj
	{
	public:
		Actor(const s2::SprPtr& spr);
		~Actor();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	public:
		const PackNode* m_node;
		const PackTrans m_trans;

	}; // Actor

	class Lerp : public cu::RefCountObj
	{
	public:
		Lerp(s2::AnimLerp::SprData type, const s2::ILerp& data);

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;

	private:
		static int GetLerpDataSize(int lerp_type);

	public:
		s2::AnimLerp::SprData m_spr_data;
		std::unique_ptr<s2::ILerp> m_lerp;

	}; // Lerp

	class Frame : public cu::RefCountObj
	{
	public:
		Frame(const s2::AnimSymbol::Frame& frame);
		~Frame();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr) const;	

	public:
		int m_index;
		std::vector<const Actor*> m_actors;
		bool m_tween;
		std::vector<const Lerp*> m_lerps;

	}; // Frame

	class Layer : public cu::RefCountObj
	{
	public:
		Layer(const s2::AnimSymbol::Layer& layer);
		~Layer();

		void PackToLuaString(ebuilder::CodeGenerator& gen) const;

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