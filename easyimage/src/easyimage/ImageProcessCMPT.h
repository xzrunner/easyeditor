#ifndef _EASYIMAGE_IMAGE_PROCESS_CMPT_H_
#define _EASYIMAGE_IMAGE_PROCESS_CMPT_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;
class EdgeDetectionShader;

class ImageProcessCMPT : public d2d::AbstractEditCMPT
{
public:
	ImageProcessCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void OnNormal(wxCommandEvent& event);

	void OnEdgeDetection(wxCommandEvent& event);
	void OnSetBlend(wxScrollEvent& event);

	void OnRelief(wxCommandEvent& event);

private:
	EdgeDetectionShader* m_edge_shader;

}; // ImageProcessCMPT

}

#endif // _EASYIMAGE_IMAGE_PROCESS_CMPT_H_