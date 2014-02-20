
#ifndef EPARTICLE_TASK_H
#define EPARTICLE_TASK_H

#include <drag2d.h>
#include <wx/splitter.h>

namespace eparticle
{
	class Task
	{
	public:

		virtual void loadFromTextFile(const char* filename);
		virtual void storeToTextFile(const char* filename) const;

		virtual void clear();

		d2d::GLCanvas* getCanvas() const;

		static Task* create(wxFrame* parent)
		{
			return new Task(parent);
		}

	protected:
		Task(wxFrame* parent);
		~Task();

		virtual void initWindows(wxSplitterWindow* leftVerticalSplitter, 
			wxSplitterWindow* rightVerticalSplitter, wxWindow*& library,
			wxWindow*& stage, wxWindow*& toolbar);

	private:
		void initLayout();

	private:
		wxWindow* m_root;

		wxFrame* m_parent;

	}; // Task
}

#endif // EPARTICLE_TASK_H
