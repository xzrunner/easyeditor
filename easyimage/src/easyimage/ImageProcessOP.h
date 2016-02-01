#ifndef _EASYIMAGE_IMAGE_PROCESS_OP_H_
#define _EASYIMAGE_IMAGE_PROCESS_OP_H_

#include <ee/ZoomViewOP.h>

namespace eimage
{

class StagePanel;

class ImageProcessOP : public ee::ZoomViewOP
{
public:
	ImageProcessOP(StagePanel* stage);

	virtual bool OnActive();

private:
	StagePanel* m_stage;

}; // ImageProcessOP

}

#endif // _EASYIMAGE_IMAGE_PROCESS_OP_H_