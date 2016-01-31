#ifndef _EASYANIM_EDIT_EKY_FRAMES_AOP_H_
#define _EASYANIM_EDIT_EKY_FRAMES_AOP_H_



namespace eanim
{

class Layer;
class KeyFrame;

class EditKeyFramesAOP : public ee::AtomicOP
{
public:
	EditKeyFramesAOP(Layer* layer);
	virtual ~EditKeyFramesAOP();

	virtual void Undo();
	virtual void Redo();
	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprites) const { return Json::Value(); }

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