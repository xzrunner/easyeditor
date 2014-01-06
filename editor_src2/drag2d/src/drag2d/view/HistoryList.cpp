#include "HistoryList.h"

#include "history/AbstractAtomicOP.h"

namespace d2d
{

HistoryList::HistoryList()
	: m_savedOP(NULL)
{
}

void HistoryList::clear()
{
	clear(m_undoStack);
	clear(m_redoStack);
}

HistoryList::Type HistoryList::undo()
{
	if (m_undoStack.empty())
		return NO_CHANGE;

	AbstractAtomicOP* op = m_undoStack.top();
	m_undoStack.pop();
	op->undo();
	m_redoStack.push(op);

	if (!m_savedOP && m_undoStack.empty())
		return NOT_FIXED;
	else if (m_savedOP && !m_undoStack.empty() && m_savedOP == m_undoStack.top())
		return NOT_FIXED;
	else
		return FIXED;
}

HistoryList::Type HistoryList::redo()
{
	if (m_redoStack.empty())
		return NO_CHANGE;

	AbstractAtomicOP* op = m_redoStack.top();
	m_redoStack.pop();
	op->redo();
	m_undoStack.push(op);

	if (!m_savedOP && m_undoStack.empty())
		return NOT_FIXED;
	else if (m_savedOP && !m_undoStack.empty() && m_savedOP == m_undoStack.top())
		return NOT_FIXED;
	else
		return FIXED;
}

void HistoryList::insert(AbstractAtomicOP* op)
{
	m_undoStack.push(op);
	clear(m_redoStack);
}

void HistoryList::onSave()
{
	if (!m_undoStack.empty())
		m_savedOP = m_undoStack.top();
}

void HistoryList::clear(std::stack<AbstractAtomicOP*>& stack)
{
	while (!stack.empty())
	{
		delete stack.top();
		stack.pop();
	}
}

}