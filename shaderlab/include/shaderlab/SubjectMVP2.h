#ifndef _SHADERLAB_SUBJECT_MVP2_H_
#define _SHADERLAB_SUBJECT_MVP2_H_

#include "SubjectMVP.h"

namespace sl
{

class SubjectMVP2 : public SubjectMVP
{
public:
	void NotifyModelview(float x, float y, float sx, float sy);
	void NotifyProjection(int width, int height);

	static SubjectMVP2* Instance();

private:
	SubjectMVP2();

private:
	static SubjectMVP2* m_instance;

};	// SubjectMVP2
	
}

#endif // _SHADERLAB_SUBJECT_MVP2_H_