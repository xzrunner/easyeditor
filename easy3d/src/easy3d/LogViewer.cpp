#include "LogViewer.h"

namespace e3d
{

LogViewer* LogViewer::m_instance = NULL;

LogViewer::LogViewer()
{
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
	//d2d::Matrix t;
	//for (int i = 0, n = m_log.size(); i < n; ++i)
	//{
	//	etext::GTxt::Instance()->Draw(t, m_log[i]);
	//	t.translate(0, 20);
	//}

	//m_log.clear();
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