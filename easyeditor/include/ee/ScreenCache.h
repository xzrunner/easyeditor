//#ifndef _EASYEDITOR_SCREEN_CACHE_H_
//#define _EASYEDITOR_SCREEN_CACHE_H_
//
//namespace ee
//{
//
//class Camera;
//
//class ScreenCache
//{
//public:
//	void SetSize(int w, int h);
//	void GetSize(int& w, int& h) const;
//
//	void Reload();
//
//	void Bind();
//	void Unbind();
//
//	void InvalidRect(float xmin, float ymin, float xmax, float ymax);
//	void Draw(const Camera* cam);
//
//	void DrawToScreen(void (*before_draw)(void* ud), void* ud);
//
//	int GetTexID() const;
//
//	void EnableColGrading(bool enable) { m_use_col_crading = enable; }
//	bool IsColGradingEnable() const { return m_use_col_crading; }
//
//	static ScreenCache* Instance();
//
//private:
//	ScreenCache();
//
//private:
//	int m_width, m_height;
//
//	bool m_use_col_crading;
//
//private:
//	static ScreenCache* m_instance;
//
//}; // ScreenCache
//
//}
//
//#endif // _EASYEDITOR_SCREEN_CACHE_H_
