#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include <wx/thread.h>
#include <wx/event.h>

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_MYTHREAD, -1)
END_DECLARE_EVENT_TYPES()

class MyThread : public wxThread
{
    public:
        MyThread();
    private:
        int m_param;
        void* Entry();
    protected:
        wxEvtHandler* m_pParent;
};
#endif 