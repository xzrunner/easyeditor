#include "EditKeyFramesAOP.h"

#include "dataset/KeyFrame.h"

namespace eanim
{

EditKeyFramesAOP::EditKeyFramesAOP(Layer* layer)
	: m_layer(layer)
{
}

EditKeyFramesAOP::~EditKeyFramesAOP()
{
	for_each(m_inserted.begin(), m_inserted.end(), d2d::ReleaseObjectFunctor<KeyFrame>());
	for_each(m_removed.begin(), m_removed.end(), d2d::ReleaseObjectFunctor<KeyFrame>());
	for (int i = 0, n = m_changed.size(); i < n; ++i) {
		m_changed[i].frame->Release();
	}
}

void EditKeyFramesAOP::undo()
{
	
}

void EditKeyFramesAOP::redo()
{
	
}

void EditKeyFramesAOP::AddRemoved(KeyFrame* kf) 
{ 
	if (kf) {
		kf->Retain();
		m_removed.push_back(kf); 
	}
}

void EditKeyFramesAOP::AddInserted(KeyFrame* kf) 
{ 
	if (kf) {
		kf->Retain();
		m_inserted.push_back(kf); 
	}
}

void EditKeyFramesAOP::AddChanged(KeyFrame* kf, int to)
{
	if (!kf) {
		return;
	}

	kf->Retain();

	ChangeFrame cf;
	cf.frame = kf;
	cf.from = kf->getTime();
	cf.to = to;
	m_changed.push_back(cf);
}

}