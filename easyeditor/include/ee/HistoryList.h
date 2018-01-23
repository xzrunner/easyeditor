#ifndef _EASYEDITOR_HISTORY_LIST_H_
#define _EASYEDITOR_HISTORY_LIST_H_

#include "Sprite.h"

#include <json/json.h>

#include <stack>

namespace ee
{

class AtomicOP;

class HistoryList
{
public:
	HistoryList();

	enum Type {
		NO_CHANGE = 0,
		DIRTY,
		NOT_DIRTY
	};

	void Clear();

	Type Undo();
	Type Redo();
	Type RedoTop();
	void Insert(const std::shared_ptr<AtomicOP>& op);

	void OnSave();

	void Store(Json::Value& value, const std::vector<SprPtr>& sprs);
	void Load(const Json::Value& value, const std::vector<SprPtr>& sprs);

private:
	void Clear(std::stack<std::shared_ptr<AtomicOP>>& stack);

	void Store(std::stack<std::shared_ptr<AtomicOP>>& stack, Json::Value& val,
		const std::vector<SprPtr>& sprs);
	void Load(std::stack<std::shared_ptr<AtomicOP>>& stack, const Json::Value& val, 
		const std::vector<SprPtr>& sprs);

private:
	std::stack<std::shared_ptr<AtomicOP>> m_undo_stack, m_redo_stack;

	std::shared_ptr<AtomicOP> m_saved_op;

}; // HistoryList

}

#endif // _EASYEDITOR_HISTORY_LIST_H_