#ifndef _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_
#define _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_



namespace eimage
{

class ExtractOutlineFine
{
public:
	ExtractOutlineFine(const std::vector<ee::Vector>& raw_border,
		const std::vector<ee::Vector>& raw_border_merged);

	void Trigger(float area_tol, float perimeter_tol);
	void Trigger(float area_tol, float perimeter_tol, int max_step);

	void CreateOutline(float area_tol, float perimeter_tol, int max_step);
	void ReduceOutlineCount(float area_tol, float perimeter_tol);

	const std::vector<ee::Vector>& GetResult() const {
		return m_fine_border;
	}

private:
	void OutlineByAddNode(float area_tol, float perimeter_tol, int max_step, bool reduce_count);

	void RemoveOneNode(int idx, ee::Vector& new0, ee::Vector& new1, float& decrease) const;
	void AddOneNode(int idx, ee::Vector& new_start, ee::Vector& new_end, ee::Vector& new_node, float& decrease) const;

	bool IsCutTriLegal(const ee::Vector& center, const ee::Vector& p0, const ee::Vector& p1) const;
	bool IsAddTriLeagal(const ee::Vector& p0, const ee::Vector& p1, const ee::Vector& p2) const;

	// start_scale [in & out], end_scale [in & out], mid [out], score [in & out]
	void StartPosExplore(const ee::Vector& p0, const ee::Vector& p1,
		float& start_scale, float& end_scale, ee::Vector& mid, float& score) const;
	// end_scale [in & out], mid [out], score [in & out]
	void EndPosExplore(float step, const ee::Vector& p0, const ee::Vector& p1, float start_scale, 
		float& end_scale, ee::Vector& mid, float& score) const;
	// mid [out], score [out]
	void MidPosExplore(const ee::Vector& start, const ee::Vector& end, 
		ee::Vector& mid, float& score) const;

	// decrease the size of m_fine_border
	void ReduceNode(float tolerance);
	void ReduceEdge(float area_tol, float perimeter_tol);

	void InitRotateLUT();

private:
	const std::vector<ee::Vector>& m_raw_border;
	const std::vector<ee::Vector>& m_raw_border_merged;

	std::vector<ee::Vector> m_fine_border;

	std::vector<ee::Vector> m_rotate_lut;

}; // ExtractOutlineFine

}

#endif // _EASYIMAGE_EXTRACT_OUTLINE_FINE_H_