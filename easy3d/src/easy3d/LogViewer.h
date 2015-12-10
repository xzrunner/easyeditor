#ifndef _E3D_LOG_VIEWER_H_
#define _E3D_LOG_VIEWER_H_

#include <drag2d.h>

namespace e3d
{

class LogViewer
{
public:
	void Add(const std::string& msg);

	void Draw() const;

public:
	static LogViewer* Instance();

private:
	LogViewer();
	~LogViewer();

private:
	mutable std::vector<std::string> m_log;

private:
	static LogViewer* m_instance;

}; // LogViewer

}

#endif // _E3D_LOG_VIEWER_H_