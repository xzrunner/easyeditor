#ifndef _EASYIMAGE_IMAGE_PROCESS_CMPT_H_
#define _EASYIMAGE_IMAGE_PROCESS_CMPT_H_



namespace eimage
{

class StagePanel;
class EdgeDetectionShader;
class BlurShader;

class ImageProcessCMPT : public ee::EditCMPT
{
public:
	ImageProcessCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnNormal(wxCommandEvent& event);

	void OnEdgeDetection(wxCommandEvent& event);
	void OnSetEdgeBlend(wxScrollEvent& event);

	void OnRelief(wxCommandEvent& event);

	void OnOutline(wxCommandEvent& event);

	void OnBlur(wxCommandEvent& event);
	void OnSetBlurRadius(wxScrollEvent& event);

private:
	EdgeDetectionShader* m_edge_shader;
	BlurShader* m_blur_shader;

}; // ImageProcessCMPT

}

#endif // _EASYIMAGE_IMAGE_PROCESS_CMPT_H_