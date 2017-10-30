#ifndef _EASYEDITOR_DIALOG_STAGE_CANVAS_H_
#define _EASYEDITOR_DIALOG_STAGE_CANVAS_H_

#include "CameraCanvas.h"
#include "Symbol.h"

#include <memory>

namespace ee
{

class DialogStageCanvas : public CameraCanvas
{
public:
	DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
		const SymPtr& sym, wxGLContext* glctx);

protected:
	virtual void OnDrawSprites() const;

private:
	std::shared_ptr<Symbol> m_sym;

}; // DialogStageCanvas

}

#endif // _EASYEDITOR_DIALOG_STAGE_CANVAS_H_