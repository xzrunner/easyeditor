#pragma once

#include "ISprite.h"

namespace d2d
{
	class NullSprite : public ISprite
	{
	public:
		NullSprite(ISymbol* symbol) 
			: m_symbol(symbol) {
				m_symbol->Retain();
		}
		~NullSprite() {
			if (m_symbol) {
				m_symbol->Release();
			}
		}

		//
		// IObject interface
		//
		virtual NullSprite* Clone() const { return NULL; }

		//
		// ISprite interface
		//
		virtual bool Update(int version) { return false; }
		virtual const ISymbol& GetSymbol() const { return *m_symbol; }
		virtual void SetSymbol(ISymbol* symbol) { m_symbol = symbol; }

	private:
		ISymbol* m_symbol;

	}; // NullSprite
}