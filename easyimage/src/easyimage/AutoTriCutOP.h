#ifndef _EASYIMAGE_AUTO_TRI_CUT_OP_H_
#define _EASYIMAGE_AUTO_TRI_CUT_OP_H_

#include <drag2d.h>

namespace eimage
{

class AutoTriCutOP : public d2d::ZoomViewOP
{
public:
	AutoTriCutOP(d2d::EditPanel* stage);

	virtual bool onActive();

	virtual bool onDraw() const;

private:
	std::vector<d2d::Vector> m_raw_bound_line;
	std::vector<d2d::Vector> m_raw_bound_line_merged;
	std::vector<d2d::Vector> m_raw_bound_points;

	std::vector<d2d::Vector> m_fine_bound_line;

	friend class AutoTriCutCMPT;

}; // AutoTriCutOP

}

#endif // _EASYIMAGE_AUTO_TRI_CUT_OP_H_