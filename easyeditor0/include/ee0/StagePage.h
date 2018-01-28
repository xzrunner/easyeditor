#pragma once

#include "ee0/Observer.h"
#include "ee0/SubjectMgr.h"

#include <ee/EditPanel.h>

namespace ee0
{

class StagePage : public ee::EditPanel, public Observer
{
public:
	StagePage(wxWindow* parent, wxTopLevelWindow* frame)
		: ee::EditPanel(parent, frame) {}

	ee0::SubjectMgr& GetSubjectMgr() { return m_sub_mgr; }

protected:
	SubjectMgr m_sub_mgr;

}; // StagePage

}