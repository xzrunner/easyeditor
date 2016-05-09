#ifndef _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_

#include <SM_Vector.h>

#include <vector>

namespace eimage
{

class ExtractOutlineFine
{
public:
	ExtractOutlineFine(const std::vector<sm::vec2>& raw_border,
		const std::vector<sm::vec2>& raw_border_merged);

	void Trigger(float area_tol, float perimeter_tol);
	void Trigger(float area_tol, float perimeter_tol, int max_step);

	void CreateOutline(float area_tol, float perimeter_tol, int max_step);
	void ReduceOutlineCount(float area_tol, float perimeter_tol);

	const std::vector<sm::vec2>& GetResult() const {
		return m_fine_border;
	}

private:
	void OutlineByAddNode(float area_tol, float perimeter_tol, int max_step, bool reduce_count);

	void RemoveOneNode(int idx, sm::vec2& new0, sm::vec2& new1, float& decrease) const;
	void AddOneNode(int idx, sm::vec2& new_start, sm::vec2& new_end, sm::vec2& new_node, float& decrease) const;

	bool IsCutTriLegal(const sm::vec2& center, const sm::vec2& p0, const sm::vec2& p1) const;
	bool IsAddTriLeagal(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2) const;

	// start_scale [in & out], end_scale [in & out], mid [out], score [in & out]
	void StartPosExplore(const sm::vec2& p0, const sm::vec2& p1,
		float& start_scale, float& end_scale, sm::vec2& mid, float& score) const;
	// end_scale [in & out], mid [out], score [in & out]
	void EndPosExplore(float step, const sm::vec2& p0, const sm::vec2& p1, float start_scale, 
		float& end_scale, sm::vec2& mid, float& score) const;
	// mid [out], score [out]
	void MidPosExplore(const sm::vec2& start, const sm::vec2& end, 
		sm::vec2& mid, float& score) const;

	// decrease the size of m_fine_border
	void ReduceNode(float tolerance);
	void ReduceEdge(float area_tol, float perimeter_tol);

	void InitRotateLUT();

private:
	const std::vector<sm::vec2>& m_raw_border;
	const std::vector<sm::vec2>& m_raw_border_merged;

	std::vector<sm::vec2> m_fine_border;

	std::vector<sm::vec2> m_rotate_lut;

}; // ExtractOutlineFine

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_