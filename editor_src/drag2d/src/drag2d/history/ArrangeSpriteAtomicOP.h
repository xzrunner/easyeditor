#pragma once

#include <vector>

#include "AbstractAtomicOP.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiSpritesImpl;

	namespace arrange_sprite
	{
		class SpritesAOP : public AbstractAtomicOP
		{
		public:
			SpritesAOP(const SpriteSelection& selection);
			SpritesAOP(const std::vector<ISprite*>& sprites);
			virtual ~SpritesAOP();

		protected:
			std::vector<ISprite*> m_sprites;

		}; // SpritesAOP

		class MoveSpritesAOP : public SpritesAOP
		{
		public:
			MoveSpritesAOP(const SpriteSelection& selection, const Vector& offset);
			MoveSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& offset);

			virtual void undo();
			virtual void redo();

		private:
			Vector m_offset;

		}; // MoveSpritesAOP

		class RotateSpritesAOP : public SpritesAOP
		{
		public:
			RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end);
			RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end);

			virtual void undo();
			virtual void redo();

		private:
			Vector m_start, m_end;

		}; // RotateSpritesAOP

		class DeleteSpritesAOP : public SpritesAOP
		{
		public:
			DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl);
			DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl);

			virtual void undo();
			virtual void redo();

		private:
			MultiSpritesImpl* m_spritesImpl;

		}; // DeleteSpritesAOP
	}
}

