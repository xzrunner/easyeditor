#ifndef _D2D_RENDER_CONTEXT_H_
#define _D2D_RENDER_CONTEXT_H_

namespace d2d
{

class ShaderMgrBase;

class RenderContext
{
public:
	static void SetShader2DMgr(ShaderMgrBase* mgr) {
		m_mgr2 = mgr;
	}
	static void SetShader3DMgr(ShaderMgrBase* mgr) {
		m_mgr3 = mgr;
	}

	static void Reload();
	static void Flush();
	static void Reset();

	static void Bind2d();
	static void Bind3d();

private:
	static void Bind(ShaderMgrBase* mgr);

private:
	static ShaderMgrBase* m_curr;
	static ShaderMgrBase* m_mgr2;
	static ShaderMgrBase* m_mgr3;

}; // RenderContext

}

#endif // _D2D_RENDER_CONTEXT_H_