#include "Layer.h"
#include "KeyFrame.h"

#include "view/KeysPanel.h"
#include "view/EditKeyFramesAOP.h"
#include "message/messages.h"

namespace eanim
{

Layer::Layer()
	: m_sprite_observer(*this)
{
	static int count = 0;
	m_name = "Layer" + d2d::StringTools::ToString(count++);

	m_editable = m_visible = true;
}

Layer::~Layer()
{
	Clear();
}

bool Layer::IsKeyFrame(int time) const
{
	return m_frames.find(time) != m_frames.end();
}

d2d::AbstractAtomicOP* Layer::RemoveFrameRegion(int begin, int end)
{
	if (begin > end || begin < 1 || end < 1) {
		return NULL;
	}

	int before_len = GetMaxFrameTime();
	if (before_len == 0) {
		return NULL;
	}

	EditKeyFramesAOP* aop = new EditKeyFramesAOP(this);

	std::vector<KeyFrame*> frames;

	bool del_first = false;

	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first < begin) {
			++itr;
			continue;
		} else if (itr->first >= begin && itr->first <= end) {
			if (itr == m_frames.begin()) {
				del_first = true;
			}
			aop->AddRemoved(itr->second);
			itr->second->Release();
			itr = m_frames.erase(itr);
		} else {
			assert(itr->first > end);
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		}
	}

	int cut_len = end - begin + 1;
	if (del_first && !frames.empty()) {
		cut_len += frames[0]->GetTime() - 2;
	}
	for (int i = 0, n = frames.size(); i < n; ++i) {
		KeyFrame* frame = frames[i];
		int new_time = frame->GetTime() - cut_len;
		aop->AddChanged(frame, new_time);
		frame->SetTime(new_time);
		InsertKeyFrame(frame->GetTime(), frame);
		frame->Release();
	}

	int after_len = GetMaxFrameTime();
	if (after_len < before_len - cut_len) {
		InsertKeyFrame(before_len - cut_len);
		aop->AddInserted(GetEndFrame());
	}

	SetSelectedSJ::Instance()->Set(-1, GetMaxFrameTime());

	return aop;
}

void Layer::InsertNullFrame(int time)
{
	std::vector<KeyFrame*> frames;
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first >= time) {
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		} else {
			++itr;
		}
	}

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		KeyFrame* frame = frames[i];
		frame->SetTime(frame->GetTime() + 1);
		InsertKeyFrame(frame->GetTime(), frame);
		frame->Release();
	}
}

void Layer::RemoveNullFrame(int time)
{
	if (IsKeyFrame(time)) {
		return;
	}

	std::vector<KeyFrame*> frames;
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first >= time) {
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		} else {
			++itr;
		}
	}

	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		KeyFrame* frame = frames[i];
		frame->SetTime(frame->GetTime() - 1);
		InsertKeyFrame(frame->GetTime(), frame);
		frame->Release();
	}
}

void Layer::InsertKeyFrame(KeyFrame* frame)
{
	std::pair<std::map<int, KeyFrame*>::iterator, bool> status 
		= InsertKeyFrame(frame->GetTime(), frame);
	// replace
	if (!status.second && frame != status.first->second)
	{
		KeyFrame* old_frame = status.first->second;
		old_frame->Release();
		status.first->second = frame;
	}

	SetSelectedSJ::Instance()->Set(-1, frame->GetTime() - 1);
}

void Layer::InsertKeyFrame(int time)
{
	if (!m_frames.empty())
	{
		if (GetMaxFrameTime() < time)
		{
			KeyFrame* frame = new KeyFrame(time);
			frame->CopyFromOther(GetEndFrame());
			InsertKeyFrame(time, frame);
			frame->Release();
		}
		else
		{
			InsertKeyFrame(time, new KeyFrame(time));
		}
		SetSelectedSJ::Instance()->Set(-1, time - 1);
	}
	else
	{
		InsertKeyFrame(1, new KeyFrame(1));
	}
}

void Layer::RemoveKeyFrame(int time)
{
	if (!IsKeyFrame(time)) {
		return;
	}

	std::map<int, KeyFrame*>::iterator itr = m_frames.find(time);
	assert(itr != m_frames.end());

	itr->second->Release();
	m_frames.erase(itr);

	int frame_idx = 0;
	KeyFrame* frame = GetCurrKeyFrame(time);
	if (frame) {
		frame_idx = frame->GetTime() - 1;
	}
	SetSelectedSJ::Instance()->Set(-1, frame_idx);
}

void Layer::ChangeKeyFrame(KeyFrame* frame, int to)
{
	if (!frame || to < 1) {
		return;
	}
	if (frame->GetTime() == to) {
		return;
	}

	std::map<int, KeyFrame*>::iterator itr = m_frames.find(frame->GetTime());
	if (itr == m_frames.end() || itr->second != frame) {
		return;
	}
	m_frames.erase(itr);

	frame->SetTime(to);
	InsertKeyFrame(frame->GetTime(), frame);
	frame->Release();
}

KeyFrame* Layer::GetCurrKeyFrame(int time)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.lower_bound(time);
	if (itr == m_frames.end()) return NULL;

	if (itr->first == time) 
	{
		return itr->second;
	}
	else
	{
		if (itr == m_frames.begin()) return NULL;

		--itr;
		assert(itr->first <= time);
		if (itr->first > time) return NULL;

		return itr->second;
	}
}

KeyFrame* Layer::GetNextKeyFrame(int time)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.upper_bound(time);
	if (itr == m_frames.end()) {
		return NULL;
	} else {
		return itr->second;
	}
}

KeyFrame* Layer::GetPrevKeyFrame(int time)
{
	if (m_frames.empty()) return NULL;

	std::map<int, KeyFrame*>::iterator itr = m_frames.lower_bound(time);
	if (itr == m_frames.end() || itr == m_frames.begin()) {
		return NULL;
	} else {
		return (--itr)->second;
	}
}

int Layer::GetMaxFrameTime() const
{
	if (m_frames.empty()) {
		return 0; 
	} else {
		return (--m_frames.end())->first;
	}
}

KeyFrame* Layer::GetEndFrame() const
{
	if (m_frames.empty()) {
		return NULL;
	} else {
		return (--m_frames.end())->second;
	}
}

void Layer::Clear()
{
	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); ++itr) {
		itr->second->Release();
	}
	m_frames.clear();
}

std::pair<std::map<int, KeyFrame*>::iterator, bool> 
Layer::InsertKeyFrame(int index, KeyFrame* frame)
{
	frame->SetLayer(this);

	std::pair<std::map<int, KeyFrame*>::iterator, bool> itr
		= m_frames.insert(std::make_pair(index, frame));
	if (itr.second) {
		frame->Retain();
	}
	return itr;
}

} // eanim