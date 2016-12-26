#ifndef _SHADERLAB_SUBJECT_MVP3_H_
#define _SHADERLAB_SUBJECT_MVP3_H_

#include "SubjectMVP.h"

namespace sl
{

class SubjectMVP3 : public SubjectMVP
{
public:
	void NotifyModelview(const sm::mat4& mat);
	void NotifyProjection(const sm::mat4& mat);

	static SubjectMVP3* Instance();

private:
	SubjectMVP3();

private:
	static SubjectMVP3* m_instance;

};	// SubjectMVP3
	
}

#endif // _SHADERLAB_SUBJECT_MVP3_H_