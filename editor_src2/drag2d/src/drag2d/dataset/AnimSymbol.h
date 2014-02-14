#pragma once

#include <vector>

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class AnimSymbol : public ISymbol
	{
	public:
		struct Frame
		{
//			int id;
			int index;
			std::vector<ISprite*> sprites;
			bool bClassicTween;
		};

		struct Layer
		{
			std::string name;
			std::vector<Frame*> frames;

			Frame* getCurrFrame(int index) const;
			Frame* getNextFrame(int index) const;
		};

	public:
		AnimSymbol();
		virtual ~AnimSymbol();

		//
		// ICloneable interface
		//
		virtual AnimSymbol* clone() const { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual Rect getSize(const ISprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh();

		size_t getMaxFrameIndex() const;

	protected:
		virtual void loadResources();

	private:
		void initBounding();

		void refreshThumbnail();

	public:
		std::vector<Layer*> m_layers;

		Rect m_rect;

		int m_fps;

	}; // AnimSymbol
}

