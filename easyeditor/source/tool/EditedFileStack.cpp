#include "EditedFileStack.h"

namespace ee
{

SINGLETON_DEFINITION(EditedFileStack)

EditedFileStack::EditedFileStack()
{
}

void EditedFileStack::Push(const std::string& filepath)
{
	m_files.push_back(filepath);
}

void EditedFileStack::Pop()
{
	if (!m_files.empty()) {
		m_files.pop_back();
	}
}

std::string EditedFileStack::Top() const
{
	return m_files.empty() ? "" : m_files.back();
}

void EditedFileStack::Clear()
{
	m_files.clear();
}

std::string EditedFileStack::GetBottom() const
{
	return m_files.empty() ? "" : m_files.front();
}

void EditedFileStack::SetBottom(const std::string& filepath)
{
	if (m_files.empty()) {
		m_files.push_back(filepath);
	} else {
		m_files[0] = filepath;
	}
}

}