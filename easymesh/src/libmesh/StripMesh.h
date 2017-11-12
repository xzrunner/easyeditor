//#ifndef _EASYMESH_STRIP_MESH_H_
//#define _EASYMESH_STRIP_MESH_H_
//
//#include <polymesh/StripMesh.h>
//
//namespace emesh
//{
//
//class StripMesh : public pm::StripMesh
//{
//public:
//	StripMesh();
//
//// 	/**
//// 	 *  @interface
//// 	 *    Mesh
//// 	 */
//// 	virtual void Load(const Json::Value& value) override;
//// 	virtual void Store(Json::Value& value) const override;
//// 	virtual void OffsetUV(float dx, float dy) override;
//
//	/**
//	 *  @interface
//	 *    Editable
//	 */
//	virtual bool RemoveMesh(ee::Shape* shape) override { return false; }
//	virtual bool InsertMesh(ee::Shape* shape) override { return false; }
//	virtual bool ClearMesh() override { return false; }
//	virtual void Reset() override;
//	virtual void Clear() override;
//
//	void InsertNode(const sm::vec2& p);
//	void RemoveNode(const sm::vec2& p);
//	sm::vec2* FindNode(const sm::vec2& p);
//	void MoveNode(sm::vec2* src, const sm::vec2& dst);
//
//	static std::string GetTypeName() { return "strip"; }
//
//private:
////	void InitBound();
//	
//// 	void RefreshTriangles();
//// 	void CopyTriangles(const StripMesh& strip);
//
//	void AbsorbNodeToRegion(sm::vec2& node);
//	void RemoveCornerFromNodes();
//
//	void GetTransList(std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list) const;
//
//	static void TranslateNode(s2::MeshNode* node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);
//	static void TranslateNode(sm::vec2& node, const std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);
//
//	static void MapUV2XY(const std::vector<sm::vec2>& nodes, int index, const sm::vec2& pos, 
//		std::vector<std::pair<sm::vec2, sm::vec2> >& trans_list);
//
//private:
//	float m_uv_offset;
//
//}; // StripMesh 
//
//}
//
//#endif // _EASYMESH_STRIP_MESH_H_