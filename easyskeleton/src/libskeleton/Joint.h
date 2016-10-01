#ifndef _LIBSKELETON_JOINT_H_
#define _LIBSKELETON_JOINT_H_

#include <sprite2/Joint.h>

namespace s2 { class Sprite; class LocalPose; }

namespace libskeleton
{

class Joint : public s2::Joint
{
public:
	Joint(s2::Sprite* spr, const s2::LocalPose& joint_pose);

	void DrawSkeleton(const s2::RenderParams& params, bool selected = false) const;

	const s2::LocalPose& GetLocalPose() const { return m_local; }
	const s2::LocalPose& GetSkinPose() const { return m_skin.pose; }

}; // Joint

}

#endif // _LIBSKELETON_JOINT_H_