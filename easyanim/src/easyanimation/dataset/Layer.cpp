#include "Layer.h"
#include "KeyFrame.h"

#include "frame/Controller.h"
#include "view/KeysPanel.h"

namespace eanim
{

Layer::Layer(Controller* ctrl)
	: m_ctrl(ctrl)
	, m_sprite_observer(*this)
{
	static int count = 0;
	m_name = "Layer" + StringTools::intToString(count++);

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

void Layer::RemoveFrameRegion(int begin, int end)
{
	if (begin > end || begin < 1 || end < 1) {
		return;
	}

	int before_len = GetMaxFrame();
	if (before_len == 0) {
		return;
	}

	std::vector<KeyFrame*> frames;

	std::map<int, KeyFrame*>::iterator itr = m_frames.begin();
	for ( ; itr != m_frames.end(); )
	{
		if (itr->first < begin) {
			++itr;
			continue;
		} else if (itr->first >= begin && itr->first <= end) {
			itr->second->Release();
			itr = m_frames.erase(itr);
		} else {
			assert(itr->first > end);
			frames.push_back(itr->second);
			itr = m_frames.erase(itr);
		}
	}

	int cut_len = end - begin + 1;
	for (int i = 0, n = frames.size(); i < n; ++i) {
		KeyFrame* frame = frames[i];
		frame->setTime(frame->getTime() - cut_len);
		InsertKeyFrame(frame->getTime(), frame);
		frame->Release();
	}

	int after_len = GetMaxFrame();
	if (after_len < before_len - cut_len) {
		InsertKeyFrame(before_len - cut_len);
	}

	m_ctrl->setCurrFrame(m_ctrl->layer(), GetMaxFrame());
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
		frame->setTime(frame->getTime() + 1);
		InsertKeyFrame(frame->getTime(), frame);
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
		frame->setTime(frame->getTime() - 1);
		InsertKeyFrame(frame->getTime(), frame);
		frame->Release();
	}
}

void Layer::InsertKeyFrame(KeyFrame* frame)
{
	std::pair<std::map<int, KeyFrame*>::iterator, bool> status 
		= InsertKeyFrame(frame->getTime(), frame);
	// replace
	if (!status.second && frame != status.first->second)
	{
		KeyFrame* old_frame = status.first->second;
		bool refresh = m_ctrl->getCurrFrame() == old_frame;

		old_frame->Release();
		status.first->second = frame;

		if (refresh) {
			m_ctrl->UpdateCurrFrame();
		}
	}

	m_ctrl->setCurrFrame(m_ctrl->layer(), frame->getTime());
}

void Layer::InsertKeyFrame(int time)
{
	if (!m_frames.empty())
	{
		if (GetMaxFrame() < time)
		{
			KeyFrame* frame = new KeyFrame(m_ctrl, time);
			frame->CopyFromOther((--m_frames.end())->second);
			InsertKeyFrame(time, frame);
			frame->Release();
			m_ctrl->setCurrFrame(m_ctrl->layer(), time);
		}
		else
		{
			InsertKeyFrame(time, new KeyFrame(m_ctrl, time));
		}
	}
	else
	{
		InsertKeyFrame(1, new KeyFrame(m_ctrl, 1));
	}
}

void Layer::RemoveKeyFrame(int time)
{
	if (time == 1) return;

	if (!IsKeyFrame(time)) {
		return;
	}

	std::map<int, KeyFrame*>::iterator itr = m_frames.find(time);
	assert(itr != m_frames.end());

	itr->second->Release();
	m_frames.erase(itr);

	m_ctrl->setCurrFrame(m_ctrl->layer(), GetMaxFrame());
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

int Layer::GetMaxFrame() const
{
	if (m_frames.empty()) {
		return 0; 
	} else {
		return (--m_frames.end())->first;
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