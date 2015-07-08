#ifndef _DRAG2D_DIRTY_STATE_H_
#define _DRAG2D_DIRTY_STATE_H_

namespace d2d
{

class DirtyState
{
public:
	DirtyState() : m_dirty(false) {}
	virtual ~DirtyState() {}

	virtual void SetDirty() const { m_dirty = true; }

	bool IsDirty() const { return m_dirty; }

	void ResetDirty() const { m_dirty = false; }

private:
	mutable bool m_dirty;

}; // DirtyState

}

#endif // _DRAG2D_DIRTY_STATE_H_