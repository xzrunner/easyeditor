#include "ResizeOP.h"
#include "Context.h"
#include "StagePanel.h"
#include "ResizeCMPT.h"

namespace e9patch
{

ResizeOP::ResizeOP(d2d::EditPanel* editPanel)
	: lib9patch::ResizeOP(editPanel, NULL)
{
}

bool ResizeOP::onMouseLeftDown(int x, int y)
{
	d2d::Patch9Symbol* symbol = Context::Instance()->stage->getPatchSymbol();
	if (!symbol) return false;

	lib9patch::ResizeOP::setSymbol(symbol);
	return lib9patch::ResizeOP::onMouseLeftDown(x, y);
}

bool ResizeOP::onMouseLeftUp(int x, int y)
{
	d2d::Patch9Symbol* symbol = Context::Instance()->stage->getPatchSymbol();
	if (!symbol) return false;

	lib9patch::ResizeOP::setSymbol(symbol);
	return lib9patch::ResizeOP::onMouseLeftUp(x, y);
}

bool ResizeOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseDrag(x, y)) return true;

	Context* context = Context::Instance();

	d2d::Patch9Symbol* symbol = dynamic_cast<d2d::Patch9Symbol*>
		(context->stage->getPatchSymbol());
	if (m_status != e_null && symbol)
	{
		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		if (symbol->type() == d2d::Patch9Symbol::e_9Grid || 
			symbol->type() == d2d::Patch9Symbol::e_9GridHollow ||
			symbol->type() == d2d::Patch9Symbol::e_6GridUpper)
		{
			context->width = fabs(pos.x)*2;
			context->height = fabs(pos.y)*2;
		}
		else if (symbol->type() == d2d::Patch9Symbol::e_3GridHor)
		{
			context->width = fabs(pos.x)*2;
			context->height = symbol->getSize().yLength();
		}
		else if (symbol->type() == d2d::Patch9Symbol::e_3GridVer)
		{
			context->width = symbol->getSize().xLength();
			context->height = fabs(pos.y)*2;
		}
		else
		{
			return true;
		}

		context->resizeCMPT->setValue(context->width, context->height);
		symbol->resize(context->width, context->height);

		m_editPanel->Refresh();
	}

	return false;
}

bool ResizeOP::onActive()
{
	if (d2d::ZoomViewOP::onActive()) return true;

	Context::Instance()->isComposeOP = false;

	Context::Instance()->stage->rebuildPatchSymbol();

	return false;
}

bool ResizeOP::onDraw() const
{
	d2d::Patch9Symbol* symbol = Context::Instance()->stage->getPatchSymbol();
	if (!symbol) return false;

	lib9patch::ResizeOP::setSymbol(symbol);
	return lib9patch::ResizeOP::onDraw();
}
} // e9patch