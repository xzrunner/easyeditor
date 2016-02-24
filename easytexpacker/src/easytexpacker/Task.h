#ifndef _EASYTEXPACKER_TASK_H_
#define _EASYTEXPACKER_TASK_H_

class wxFrame;
class wxWindow;

namespace etexpacker
{

class Task
{
public:

	virtual void LoadFromFile(const char* filename);
	virtual void StoreToFile(const char* filename) const;

	virtual void Clear();

	static Task* Create(wxFrame* parent)
	{
		return new Task(parent);
	}

protected:Task(wxFrame* parent);
	~Task();

private:
	void InitLayout();

private:
	wxWindow* m_root;

	wxFrame* m_parent;

}; // Task

}

#endif // _EASYTEXPACKER_TASK_H_