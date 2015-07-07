#ifndef _DRAG2D_IEDIT_OP_MONITOR_H_
#define _DRAG2D_IEDIT_OP_MONITOR_H_

namespace d2d
{

class AbstractAtomicOP;

class IEditOPMonitor
{
public:
	virtual ~IEditOPMonitor() {}
	virtual void AddEditOP(AbstractAtomicOP* op) = 0;
}; // IEditOPMonitor

}

#endif // _DRAG2D_IEDIT_OP_MONITOR_H_