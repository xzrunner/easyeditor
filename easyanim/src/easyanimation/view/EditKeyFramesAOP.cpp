#include "EditKeyFramesAOP.h"

#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"

#include <algorithm>

namespace eanim
{

EditKeyFramesAOP::EditKeyFramesAOP(Layer* layer)
	: m_layer(layer)
{
}

EditKeyFramesAOP::~EditKeyFramesAOP()
{
	for_each(m_inserted.begin(), m_inserted.end(), cu::RemoveRefFunctor<KeyFrame>());
	for_each(m_removed.begin(), m_removed.end(), cu::RemoveRefFunctor<KeyFrame>());
	for (int i = 0, n = m_changed.size(); i < n; ++i) {
		m_changed[i].frame->RemoveReference();
	}
}

void EditKeyFramesAOP::Undo()
{
	for (int i = 0, n = m_inserted.size(); i < n; ++i) {
		m_layer->RemoveKeyFrame(m_inserted[i]->GetTime());
	}
	for (int i = 0, n = m_removed.size(); i < n; ++i) {
		m_layer->InsertKeyFrame(m_removed[i]);
	}
	for (int i = 0, n = m_changed.size(); i < n; ++i) {
		const ChangeFrame& change = m_changed[i];
		m_layer->ChangeKeyFrame(change.frame, change.from);
	}
}

void EditKeyFramesAOP::Redo()
{
	for (int i = 0, n = m_removed.size(); i < n; ++i) {
		m_layer->RemoveKeyFrame(m_removed[i]->GetTime());
	}
	for (int i = 0, n = m_inserted.size(); i < n; ++i) {
		m_layer->InsertKeyFrame(m_inserted[i]);
	}
	for (int i = 0, n = m_changed.size(); i < n; ++i) {
		const ChangeFrame& change = m_changed[i];
		m_layer->ChangeKeyFrame(change.frame, change.to);
	}
}

void EditKeyFramesAOP::AddRemoved(KeyFrame* kf) 
{ 
	if (kf) {
		kf->AddReference();
		m_removed.push_back(kf); 
	}
}

void EditKeyFramesAOP::AddInserted(KeyFrame* kf) 
{ 
	if (kf) {
		kf->AddReference();
		m_inserted.push_back(kf); 
	}
}

void EditKeyFramesAOP::AddChanged(KeyFrame* kf, int to)
{
	if (!kf) {
		return;
	}

	kf->AddReference();

	ChangeFrame cf;
	cf.frame = kf;
	cf.from = kf->GetTime();
	cf.to = to;
	m_changed.push_back(cf);
}

}