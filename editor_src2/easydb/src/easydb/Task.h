#ifndef EDB_TASK_H
#define EDB_TASK_H

#include <drag2d.h>
#include <wx/splitter.h>

namespace edb
{
	class Task
	{
	public:
		
		virtual void loadFromFile(const char* filename);
		virtual void storeToFile(const char* filename) const;

		virtual void clear();

		static Task* create(wxFrame* parent)
		{
			return new Task(parent);
		}

	protected:
		Task(wxFrame* parent);
		~Task();

	private:
		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

	}; // Task
}

#endif // EDB_TASK_H