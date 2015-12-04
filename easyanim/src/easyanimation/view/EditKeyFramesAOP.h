#ifndef _EASYANIM_EDIT_EKY_FRAMES_AOP_H_
#define _EASYANIM_EDIT_EKY_FRAMES_AOP_H_

#include <drag2d.h>

namespace eanim
{

class Controller;
class Layer;
class KeyFrame;

class EditKeyFramesAOP : public d2d::AbstractAtomicOP
{
public:
	EditKeyFramesAOP(Layer* layer);
	virtual ~EditKeyFramesAOP();

	virtual void undo();
	virtual void redo();
	virtual Json::Value store(const std::vector<d2d::ISprite*>& sprites) { return Json::Value(); }

	void AddRemoved(KeyFrame* kf);
	void AddInserted(KeyFrame* kf);
	void AddChanged(KeyFrame* kf, int to);

private:
	struct ChangeFrame
	{
		KeyFrame* frame;
		int from, to;
	};

private:
	Layer* m_layer;

	std::vector<KeyFrame*> m_removed, m_inserted;
	std::vector<ChangeFrame> m_changed;	

}; // EditKeyFramesAOP

}

#endif // _EASYANIM_EDIT_EKY_FRAMES_AOP_H_