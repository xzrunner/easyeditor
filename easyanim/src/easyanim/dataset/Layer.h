#pragma once



namespace eanim
{
	class KeyFrame;
	class Sprite;

	class Layer
	{
	public:
		Layer();
		~Layer();

		void loadFromTextFile(std::ifstream& fin);
		void storeToTextFile(std::ofstream& fout) const;

		void insertKeyFrame(int time, KeyFrame* frame);
		bool isKeyFrame(int time) const;
		void deleteKeyFrame(int time);

		const std::string& getName() const;
		void setName(const std::string& name);
		const std::map<int, KeyFrame*>& getAllFrames() const;

		bool insertSprite(Sprite* sprite, int currFrame);
		bool removeSprite(ee::ISprite* sprite);

		KeyFrame* getCurrKeyFrame(int currFrame);
		KeyFrame* getNextKeyFrame(int currFrame);

		int getFrameCount() const;

	private:
		void clear();

	private:
		std::string m_name;

		std::map<int, KeyFrame*> m_frames;

	}; // Layer
}

