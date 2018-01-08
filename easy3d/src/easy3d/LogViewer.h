#pragma once

#include <string>
#include <vector>

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