#ifndef _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_

#include <drag2d.h>

namespace eimage
{

class ExtractOutlineFine
{
public:
	ExtractOutlineFine(const std::vector<d2d::Vector>& raw_border,
		const std::vector<d2d::Vector>& raw_border_merged);

	void Trigger(float tolerance);

	const std::vector<d2d::Vector>& GetResult() const {
		return m_fine_border;
	}

private:
	void RemoveOneNode(int idx, d2d::Vector& new0, d2d::Vector& new1, float& decrease) const;
	void AddOneNode(int idx, float r_decrease, d2d::Vector& new_start, d2d::Vector& new_end, 
		d2d::Vector& new_node, float& decrease) const;
	bool IsSegmentLegal(const d2d::Vector& p0, const d2d::Vector& p1, const d2d::Vector& p2) const;
	bool IsSegmentLegalNew(const d2d::Vector& s0, const d2d::Vector& e0, 
		const d2d::Vector& s1, const d2d::Vector& e1) const;

private:
	const std::vector<d2d::Vector>& m_raw_border;
	const std::vector<d2d::Vector>& m_raw_border_merged;

	std::vector<d2d::Vector> m_fine_border;

}; // ExtractOutlineFine

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_