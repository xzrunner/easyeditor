#include "HistoryList.h"

#include "history/AbstractAtomicOP.h"
#include "history/AtomicFactory.h"

#include <fstream>
#include <assert.h>

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
		return NOT_DIRTY;
	else if (m_savedOP && !m_undoStack.empty() && m_savedOP == m_undoStack.top())
		return NOT_DIRTY;
	else
		return DIRTY;
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
		return NOT_DIRTY;
	else if (m_savedOP && !m_undoStack.empty() && m_savedOP == m_undoStack.top())
		return NOT_DIRTY;
	else
		return DIRTY;
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

void HistoryList::store(Json::Value& value, const std::vector<ISprite*>& sprites)
{
	store(m_undoStack, value["undo"], sprites);
	store(m_redoStack, value["redo"], sprites);
}

void HistoryList::load(const Json::Value& value, const std::vector<ISprite*>& sprites)
{
	if (!value["undo"].isNull()) {
		load(m_undoStack, value["undo"], sprites);
	}
	if (!value["redo"].isNull()) {
		load(m_redoStack, value["redo"], sprites);
	}
}

void HistoryList::clear(std::stack<AbstractAtomicOP*>& stack)
{
	while (!stack.empty())
	{
		delete stack.top();
		stack.pop();
	}
}

void HistoryList::store(std::stack<AbstractAtomicOP*>& stack, Json::Value& val,
						const std::vector<ISprite*>& sprites)
{
	std::vector<AbstractAtomicOP*> tmp;
	while (!stack.empty()) {
		tmp.push_back(stack.top());
		stack.pop();
	}

	for (int i = tmp.size() - 1; i >= 0; --i) {
		AbstractAtomicOP* op = tmp[i];
		val[tmp.size() - 1 - i] = op->store(sprites);
		stack.push(op);
	}
}

void HistoryList::load(std::stack<AbstractAtomicOP*>& stack, const Json::Value& val,
					   const std::vector<ISprite*>& sprites)
{
	int i = 0;
	Json::Value opValue = val[i++];
	while (!opValue.isNull()) {
		std::vector<ISprite*> selected;

		int j = 0;
		Json::Value spriteValue = opValue["sprites"][j++];
		while (!spriteValue.isNull()) {
			int index = spriteValue.asInt();

			// error version
			if (index < 0 || index >= sprites.size())
			{
				while (!stack.empty())
				{
					AbstractAtomicOP* op = stack.top(); stack.pop();
					delete op;
				}
				return;
			}

			assert(index >= 0 && index < sprites.size());
			selected.push_back(sprites[index]);
			spriteValue = opValue["sprites"][j++];
		}
		
		AbstractAtomicOP* op = AtomicFactory::create(opValue["val"], selected);
		if (op)
			stack.push(op);

		opValue = val[i++];
	}
}

}