#ifndef EANIM_SKELETON_DATA_H
#define EANIM_SKELETON_DATA_H

#include <drag2d.h>

namespace eanim
{
	class Joint;

	class SkeletonData
	{
	public:
		~SkeletonData();

		void copyFrom(const SkeletonData& skeleton);
		void copyFrom(const std::vector<d2d::ISprite*>& sprites,
			const SkeletonData& skeleton);

		void removeSprite(d2d::ISprite* sprite);
		bool isContainSprite(d2d::ISprite* sprite) const;

		void insertJoint(d2d::ISprite* sprite, const d2d::Vector& pos);
		void removeJoint(d2d::Vector& pos);

		Joint* queryJointByPos(const d2d::Vector& pos);

		void draw(const d2d::Screen& scr) const;

		void absorb(d2d::ISprite* sprite);
		void fixJoint(d2d::ISprite* sprite);
		void updateJoint(d2d::ISprite* sprite, float dAngle = 0);

		static void getTweenSprites(SkeletonData& start, SkeletonData& end, 
			std::vector<d2d::ISprite*>& tween, float process);

	private:
		void clean();

		void translate(d2d::ISprite* sprite, const d2d::Vector& offset);

		static d2d::ISprite* getSpriteByName(const std::vector<d2d::ISprite*>& sprites, const std::string& name);

	private:
		std::map<d2d::ISprite*, std::vector<Joint*> > m_mapJoints;

		friend class FileIO;

	}; // SkeletonData
}

#endif // EANIM_SKELETON_DATA_H