#include "HistoryList.h"
#include "AtomicOP.h"
#include "AtomicFactory.h"
#include "sprite_msg.h"

#include <fstream>

#include <assert.h>

namespace ee
{

HistoryList::HistoryList()
	: m_saved_op(NULL)
{
}

void HistoryList::Clear()
{
	Clear(m_undo_stack);
	Clear(m_redo_stack);
	m_saved_op = NULL;
}

HistoryList::Type HistoryList::Undo()
{
	if (m_undo_stack.empty())
		return NO_CHANGE;

	AtomicOP* op = m_undo_stack.top();
	m_undo_stack.pop();
	op->Undo();
	m_redo_stack.push(op);

	if (!m_saved_op && m_undo_stack.empty())
		return NOT_DIRTY;
	else if (m_saved_op && !m_undo_stack.empty() && m_saved_op == m_undo_stack.top())
		return NOT_DIRTY;
	else
		return DIRTY;
}

HistoryList::Type HistoryList::Redo()
{
	if (m_redo_stack.empty())
		return NO_CHANGE;

	AtomicOP* op = m_redo_stack.top();
	m_redo_stack.pop();
	op->Redo();
	m_undo_stack.push(op);

	if (!m_saved_op && m_undo_stack.empty())
		return NOT_DIRTY;
	else if (m_saved_op && !m_undo_stack.empty() && m_saved_op == m_undo_stack.top())
		return NOT_DIRTY;
	else
		return DIRTY;
}

HistoryList::Type HistoryList::RedoTop()
{
	if (m_undo_stack.empty()) {
		return NO_CHANGE;
	} 

	std::vector<Sprite*> sprs;
	QuerySelectedSprsSJ::Instance()->Query(sprs);

	AtomicOP* op = m_undo_stack.top();
	op->Copy(sprs);
	return DIRTY;
}

void HistoryList::Insert(AtomicOP* op)
{
	assert(op);
	m_undo_stack.push(op);
	Clear(m_redo_stack);
}

void HistoryList::OnSave()
{
	if (!m_undo_stack.empty())
		m_saved_op = m_undo_stack.top();
}

void HistoryList::Store(Json::Value& value, const std::vector<Sprite*>& sprs)
{
	Store(m_undo_stack, value["undo"], sprs);
	Store(m_redo_stack, value["redo"], sprs);
}

void HistoryList::Load(const Json::Value& value, const std::vector<Sprite*>& sprs)
{
	if (!value["undo"].isNull()) {
		Load(m_undo_stack, value["undo"], sprs);
	}
	if (!value["redo"].isNull()) {
		Load(m_redo_stack, value["redo"], sprs);
	}
}

void HistoryList::Clear(std::stack<AtomicOP*>& stack)
{
	while (!stack.empty())
	{
		delete stack.top();
		stack.pop();
	}
}

void HistoryList::Store(std::stack<AtomicOP*>& stack, Json::Value& val,
						const std::vector<Sprite*>& sprs)
{
	std::vector<AtomicOP*> tmp;
	while (!stack.empty()) {
		tmp.push_back(stack.top());
		stack.pop();
	}

	for (int i = tmp.size() - 1; i >= 0; --i) {
		AtomicOP* op = tmp[i];
		val[tmp.size() - 1 - i] = op->Store(sprs);
		stack.push(op);
	}
}

void HistoryList::Load(std::stack<AtomicOP*>& stack, const Json::Value& val,
					   const std::vector<Sprite*>& sprs)
{
	int i = 0;
	Json::Value opValue = val[i++];
	while (!opValue.isNull()) {
		std::vector<Sprite*> selected;

		int j = 0;
		Json::Value spriteValue = opValue["sprites"][j++];
		while (!spriteValue.isNull()) {
			int index = spriteValue.asInt();

			// error version
			if (index < 0 || index >= static_cast<int>(sprs.size()))
			{
				while (!stack.empty())
				{
					AtomicOP* op = stack.top(); stack.pop();
					delete op;
				}
				return;
			}

			assert(index >= 0 && static_cast<int>(index < sprs.size()));
			selected.push_back(sprs[index]);
			spriteValue = opValue["sprites"][j++];
		}
		
		AtomicOP* op = AtomicFactory::Create(opValue["val"], selected);
		if (op)
			stack.push(op);

		opValue = val[i++];
	}
}

}