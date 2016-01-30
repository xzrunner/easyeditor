#pragma once

#include <stack>
#include <json/json.h>

namespace d2d
{
	class Sprite;
	class AbstractAtomicOP;

	class HistoryList
	{
	public:
		HistoryList();

		enum Type {
			NO_CHANGE = 0,
			DIRTY,
			NOT_DIRTY
		};

		void clear();

		Type undo();
		Type redo();
		void insert(AbstractAtomicOP* op);

		void onSave();

		void store(Json::Value& value, const std::vector<Sprite*>& sprites);
		void load(const Json::Value& value, const std::vector<Sprite*>& sprites);

	private:
		void clear(std::stack<AbstractAtomicOP*>& stack);

		void store(std::stack<AbstractAtomicOP*>& stack, Json::Value& val, 
			const std::vector<Sprite*>& sprites);
		void load(std::stack<AbstractAtomicOP*>& stack, const Json::Value& val, 
			const std::vector<Sprite*>& sprites);

	private:
		std::stack<AbstractAtomicOP*> m_undoStack, m_redoStack;

		AbstractAtomicOP* m_savedOP;

	}; // HistoryList
}