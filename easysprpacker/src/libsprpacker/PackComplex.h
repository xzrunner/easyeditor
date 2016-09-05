#ifndef _EASYSPRPACKER_PACK_COMPLEX_H_
#define _EASYSPRPACKER_PACK_COMPLEX_H_

#include "PackNode.h"

#include <SM_Rect.h>

#include <vector>

namespace ecomplex { class Symbol; }

namespace esprpacker
{

class PackComplex : public PackNode
{
public:
	PackComplex(const ecomplex::Symbol* sym);
	virtual ~PackComplex();

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
	void Init(const ecomplex::Symbol* sym);

private:
	class Action
	{
	public:
		int SizeOfUnpackFromBin() const;
		int SizeOfPackToBin() const;
		void PackToBin(uint8_t** ptr, const std::vector<const PackNode*>& children) const;	

	private:
		std::string m_name;
		std::vector<const PackNode*> m_sprs;

	}; // Action

private:
	std::vector<const PackNode*> m_children;

	std::vector<Action> m_actions;

	sm::rect m_scissor;

}; // PackComplex

}

#endif // _EASYSPRPACKER_PACK_COMPLEX_H_