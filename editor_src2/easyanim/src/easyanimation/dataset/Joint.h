#ifndef EANIM_JOINT_H
#define EANIM_JOINT_H

#include <drag2d.h>

namespace eanim
{
	class Joint
	{
	public:
		Joint(d2d::ISprite* sprite);
		Joint(d2d::ISprite* sprite, const d2d::Vector& pos);
		~Joint();

		void draw() const;

		bool contain(const d2d::Vector& pos) const;
		bool intersect(const d2d::Vector& pos) const;

		void setPosition(const d2d::Vector& pos);

		d2d::Vector getWorldPos() const;
		d2d::Vector getRelativePos(const d2d::Vector& pos) const;

		void translate(const d2d::Vector& offset);

		void connect(Joint* joint);
		void deconnect();

	private:
		void createId();

	private:
		static const int REGION;

	private:
		int m_id;

		d2d::ISprite* m_sprite;

		d2d::Vector m_relative;
		float m_relativeAngle;

		Joint* m_parent;
		std::set<Joint*> m_children;

		friend class SkeletonData;
		friend class FileIO;

	}; // Joint
}

#endif // EANIM_JOINT_H