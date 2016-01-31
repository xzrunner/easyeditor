#ifndef _EASYIMAGE_AUTO_TRI_CUT_OP_H_
#define _EASYIMAGE_AUTO_TRI_CUT_OP_H_



namespace eimage
{

class AutoTriCutOP : public ee::ZoomViewOP
{
public:
	AutoTriCutOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnActive();

	virtual bool OnDraw() const;

private:
	std::vector<ee::Vector> m_raw_bound_line;
	std::vector<ee::Vector> m_raw_bound_line_merged;
	std::vector<ee::Vector> m_raw_bound_points;

	std::vector<ee::Vector> m_fine_bound_line;

	friend class AutoTriCutCMPT;

}; // AutoTriCutOP

}

#endif // _EASYIMAGE_AUTO_TRI_CUT_OP_H_