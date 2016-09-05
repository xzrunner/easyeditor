#ifndef _EASYEDITOR_HISTORY_LIST_H_
#define _EASYEDITOR_HISTORY_LIST_H_

#include <json/json.h>

#include <stack>

namespace ee
{

class Sprite;
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
	void Insert(AtomicOP* op);

	void OnSave();

	void Store(Json::Value& value, const std::vector<Sprite*>& sprs);
	void Load(const Json::Value& value, const std::vector<Sprite*>& sprs);

private:
	void Clear(std::stack<AtomicOP*>& stack);

	void Store(std::stack<AtomicOP*>& stack, Json::Value& val, 
		const std::vector<Sprite*>& sprs);
	void Load(std::stack<AtomicOP*>& stack, const Json::Value& val, 
		const std::vector<Sprite*>& sprs);

private:
	std::stack<AtomicOP*> m_undo_stack, m_redo_stack;

	AtomicOP* m_saved_op;

}; // HistoryList

}

#endif // _EASYEDITOR_HISTORY_LIST_H_