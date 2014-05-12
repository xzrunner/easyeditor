#pragma once

#include <stack>
#include <JSON/json.h>

namespace d2d
{
	class ISprite;
	class AbstractAtomicOP;

	class HistoryList
	{
	public:
		HistoryList();

		enum Type {
			NO_CHANGE = 0,
			FIXED,
			NOT_FIXED
		};

		void clear();

		Type undo();
		Type redo();
		void insert(AbstractAtomicOP* op);

		void onSave();

		void store(Json::Value& value, const std::vector<ISprite*>& sprites);
		void load(const Json::Value& value, const std::vector<ISprite*>& sprites);

	private:
		void clear(std::stack<AbstractAtomicOP*>& stack);

		void store(std::stack<AbstractAtomicOP*>& stack, Json::Value& val, 
			const std::vector<ISprite*>& sprites);
		void load(std::stack<AbstractAtomicOP*>& stack, const Json::Value& val, 
			const std::vector<ISprite*>& sprites);

	private:
		std::stack<AbstractAtomicOP*> m_undoStack, m_redoStack;

		AbstractAtomicOP* m_savedOP;

	}; // HistoryList
}