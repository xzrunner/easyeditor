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

			virtual Json::Value store(const std::vector<ISprite*>& sprites);

		protected:
			virtual Json::Value storeValues() = 0;

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

		protected:
			virtual Json::Value storeValues();

		private:
			Vector m_offset;

		}; // MoveSpritesAOP

		class RotateSpritesAOP : public SpritesAOP
		{
		public:
			RotateSpritesAOP(const SpriteSelection& selection, const Vector& start, const Vector& end);
			RotateSpritesAOP(const std::vector<ISprite*>& sprites, const Vector& start, const Vector& end);
			RotateSpritesAOP(const std::vector<ISprite*>& sprites, float angle);

			virtual void undo();
			virtual void redo();

		protected:
			virtual Json::Value storeValues();

		private:
			Vector m_start, m_end;

			float m_angle; // in rad

		}; // RotateSpritesAOP

		class DeleteSpritesAOP : public SpritesAOP
		{
		public:
			DeleteSpritesAOP(const SpriteSelection& selection, MultiSpritesImpl* spritesImpl);
			DeleteSpritesAOP(const std::vector<ISprite*>& sprites, MultiSpritesImpl* spritesImpl);

			virtual void undo();
			virtual void redo();

		protected:
			virtual Json::Value storeValues();

		private:
			MultiSpritesImpl* m_spritesImpl;

		}; // DeleteSpritesAOP

		class ScaleSpritesAOP : public SpritesAOP
		{
		public:
			ScaleSpritesAOP(const std::vector<ISprite*>& sprites, float xScale, float yScale);
			ScaleSpritesAOP(const std::vector<ISprite*>& sprites, float xScale, float yScale,
				const std::vector<std::pair<float, float> >& oldScales);

			virtual void undo();
			virtual void redo();

		protected:
			virtual Json::Value storeValues();

		private:
			Vector m_scale;
			std::vector<std::pair<float, float> > m_oldScales;

		}; // ScaleSpritesAOP

		class ShearSpritesAOP : public SpritesAOP
		{
		public:
			ShearSpritesAOP(const std::vector<ISprite*>& sprites, float xShear, float yShear);
			ShearSpritesAOP(const std::vector<ISprite*>& sprites, float xShear, float yShear,
				const std::vector<std::pair<float, float> >& oldShears);

			virtual void undo();
			virtual void redo();

		protected:
			virtual Json::Value storeValues();

		private:
			float m_xShear, m_yShear;
			std::vector<std::pair<float, float> > m_oldShears;

		}; // ShearSpritesAOP

		class MirrorSpritesAOP : public SpritesAOP
		{
		public:
			MirrorSpritesAOP(const std::vector<ISprite*>& sprites, bool xMirror, bool yMirror);
			MirrorSpritesAOP(const std::vector<ISprite*>& sprites, bool xMirror, bool yMirror,
				const std::vector<std::pair<bool, bool> >& oldMirrors);

			virtual void undo();
			virtual void redo();

		protected:
			virtual Json::Value storeValues();

		private:
			bool m_xMirror, m_yMirror;
			std::vector<std::pair<bool, bool> > m_oldMirrors;

		}; // MirrorSpritesAOP
	}
}

