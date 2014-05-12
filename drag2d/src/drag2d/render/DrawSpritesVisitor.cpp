#include "DrawSpritesVisitor.h"
#include "SpriteBatch.h"
#include "SpriteDraw.h"
#include "GL10.h"

#include "dataset/ImageSprite.h"

namespace d2d
{

DrawSpritesVisitor::DrawSpritesVisitor(SpriteBatch& batch)
	: m_batch(batch)
{
// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	m_batch.clear();
}

DrawSpritesVisitor::~DrawSpritesVisitor()
{
	m_batch.onDraw();

	//GL10::Disable(GL10::GL_BLEND);
}

void DrawSpritesVisitor::visit(Object* object, bool& bFetchNext)
{
	bFetchNext = true;

// 	ImageSprite* sprite = dynamic_cast<ImageSprite*>(object);
// 	if (sprite)
// 		m_batch.add(sprite);
// 	else
		SpriteDraw::drawSprite(static_cast<ISprite*>(object));
}

} // d2d