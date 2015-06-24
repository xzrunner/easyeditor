#include "LogViewer.h"

namespace e3d
{

LogViewer* LogViewer::m_instance = NULL;

LogViewer::LogViewer()
{
	m_style.has_edge = false;
	m_style.font_size = 20;
	m_style.width = 200;
	m_style.height = 50;
	m_style.color = d2d::LIGHT_RED;
	m_style.align_hori = d2d::HAT_LEFT;
	m_style.align_vert = d2d::VAT_TOP;
}

LogViewer::~LogViewer()
{
}

void LogViewer::Add(const std::string& msg)
{
	if (m_log.empty()) {
		m_log.push_back(msg);
	}
}

void LogViewer::Draw() const
{
	d2d::Vector pos(0, 0);
	for (int i = 0, n = m_log.size(); i < n; ++i)
	{
		d2d::LabelNew::Print(m_log[i].c_str(), pos, 1, m_style);
		pos.y += 20;
	}

	m_log.clear();
}

LogViewer* LogViewer::Instance()
{
	if (!m_instance)
	{
		m_instance = new LogViewer();
	}
	return m_instance;
}

}