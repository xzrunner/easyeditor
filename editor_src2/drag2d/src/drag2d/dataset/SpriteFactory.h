#pragma once

#include "ISymbol.h"

#include <vector>

namespace d2d
{
	class ISprite;

	class SpriteFactory
	{
	public:
		ISprite* create(ISymbol* symbol);

		void insert(ISprite* sprite);
		void remove(ISprite* sprite);

		void updateBoundings(const ISymbol& symbol);

	public:
		static SpriteFactory* Instance();

	private:
		SpriteFactory();

	private:
		typedef std::vector<ISprite*> SpriteList;
		std::map<const ISymbol*, SpriteList> m_map_symbol2sprites;

		int m_id;

	private:
		static SpriteFactory* m_instance;

	}; // SpriteFactory
}

