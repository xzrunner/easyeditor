#ifndef _DRAG2D_TWO_PASS_CANVAS_H_
#define _DRAG2D_TWO_PASS_CANVAS_H_

#include "OnePassCanvas.h"
#include "IEditOPMonitor.h"

namespace d2d
{

class TwoPassCanvas : public OnePassCanvas, public IEditOPMonitor
{
public:
	TwoPassCanvas(EditPanel* stage);

	//
	// interface IEditOPMonitor
	//
	virtual void AddEditOP(AbstractAtomicOP* op);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

private:
	mutable bool m_dirty;

}; // TwoPassCanvas

}

#endif // _DRAG2D_TWO_PASS_CANVAS_H_