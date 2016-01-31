#pragma once



namespace eanim
{
	class Sprite;

	class KeyFrame
	{
	public:
		KeyFrame(int time);
		~KeyFrame();

		void loadFromTextFile(std::ifstream& fin);
		void storeToTextFile(std::ofstream& fout) const;

		bool isEmpty() const;

		void insertSprite(Sprite* sprite);
		bool removeSprite(ee::ISprite* sprite);
		const std::vector<Sprite*>& getAllSprites() const;

		bool hasClassicTween() const;
		void setClassicTween(bool has = true);

		int getTime() const;

	private:
		void clear();

	private:
		int m_time;

		std::vector<Sprite*> m_sprites;

		bool m_bClassicTween;

	}; // KeyFrame
}

