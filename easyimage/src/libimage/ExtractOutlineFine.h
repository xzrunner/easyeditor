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

	void Trigger(float area_tol, float perimeter_tol);
	void Trigger(float area_tol, float perimeter_tol, int max_step);

	void CreateOutline(float area_tol, float perimeter_tol, int max_step);
	void ReduceOutlineCount(float area_tol, float perimeter_tol);

	const std::vector<d2d::Vector>& GetResult() const {
		return m_fine_border;
	}

private:
	void OutlineByAddNode(float area_tol, float perimeter_tol, int max_step, bool reduce_count);

	void RemoveOneNode(int idx, d2d::Vector& new0, d2d::Vector& new1, float& decrease) const;
	void AddOneNode(int idx, d2d::Vector& new_start, d2d::Vector& new_end, d2d::Vector& new_node, float& decrease) const;

	bool IsCutTriLegal(const d2d::Vector& center, const d2d::Vector& p0, const d2d::Vector& p1) const;
	bool IsAddTriLeagal(const d2d::Vector& p0, const d2d::Vector& p1, const d2d::Vector& p2) const;

	// start_scale [in & out], end_scale [in & out], mid [out], score [in & out]
	void StartPosExplore(const d2d::Vector& p0, const d2d::Vector& p1,
		float& start_scale, float& end_scale, d2d::Vector& mid, float& score) const;
	// end_scale [in & out], mid [out], score [in & out]
	void EndPosExplore(float step, const d2d::Vector& p0, const d2d::Vector& p1, float start_scale, 
		float& end_scale, d2d::Vector& mid, float& score) const;
	// mid [out], score [out]
	void MidPosExplore(const d2d::Vector& start, const d2d::Vector& end, 
		d2d::Vector& mid, float& score) const;

	// decrease the size of m_fine_border
	void ReduceNode(float tolerance);
	void ReduceEdge(float area_tol, float perimeter_tol);

private:
	const std::vector<d2d::Vector>& m_raw_border;
	const std::vector<d2d::Vector>& m_raw_border_merged;

	std::vector<d2d::Vector> m_fine_border;

}; // ExtractOutlineFine

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_