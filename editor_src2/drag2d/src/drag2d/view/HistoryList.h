#pragma once

#include <stack>

namespace d2d
{
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

	private:
		void clear(std::stack<AbstractAtomicOP*>& stack);

	private:
		std::stack<AbstractAtomicOP*> m_undoStack, m_redoStack;

		AbstractAtomicOP* m_savedOP;

	}; // HistoryList
}