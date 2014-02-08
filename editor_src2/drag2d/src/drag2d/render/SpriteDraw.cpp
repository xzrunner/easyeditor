#include "SpriteDraw.h"
#include "GL10.h"
#include "SpriteBatch.h"
#include "Shader.h"

#include "common/Math.h"
#include "common/Matrix.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/ImageSprite.h"
#include "dataset/Bitmap.h"

namespace d2d
{

float SpriteDraw::time = 0;

void SpriteDraw::drawSprite(const ISprite* sprite, const Colorf& mul, const Colorf& add)
{
	if (!sprite->visiable)
		return;

	Shader* shader = Shader::Instance();
	shader->sprite();

	GL10::PushMatrix();

	love::Matrix t;

 	bool xMirror, yMirror;
 	sprite->getMirror(xMirror, yMirror);
 	const float xScale = xMirror ? -sprite->getScaleX() : sprite->getScaleX(),
 		yScale = yMirror ? -sprite->getScaleY() : sprite->getScaleY();

 	d2d::Vector center_offset = Math::rotateVector(-sprite->getOffset(), sprite->getAngle()) + sprite->getOffset();
 	d2d::Vector center = sprite->getPosition() + center_offset;
 	t.setTransformation(center.x, center.y, sprite->getAngle(), 
 		xScale, yScale, 0, 0, sprite->getShearX(), sprite->getShearY());
 	GL10::MultMatrixf((const float*)t.getElements( ));

	Colorf m = cMul(mul, sprite->multiCol),
		a = cAdd(add, sprite->addCol);
	shader->color(m, a);

	sprite->getSymbol().draw(sprite);

	GL10::PopMatrix();
}

void SpriteDraw::drawSprite(const ISymbol* symbol, const Vector& pos,
							float angle/* = 0.0f*/, float xScale/* = 1.0f*/, 
							float yScale/* = 1.0f*/, float xShear/* = 0.0f*/, 
							float yShear/* = 0.0f*/)
{
	GL10::PushMatrix();

	love::Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	GL10::MultMatrixf((const float*)t.getElements());

	symbol->draw();
	GL10::PopMatrix();
}

void SpriteDraw::drawSprite(const ISprite* sprite, wxMemoryDC& memDC)
{
	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;

	const wxBitmap* bitmap = sprite->getSymbol().getBitmap()->getBitmap();

	const float width = std::max(1.0f, sprite->getSymbol().getWidth() * sprite->getScaleX() * SCALE),
		height = std::max(1.0f, sprite->getSymbol().getHeight() * sprite->getScaleY() * SCALE);
	const float hWidth = width * 0.5f,
		hHeight = height * 0.5f;

	float xMin, yMin;
	xMin = yMin = FLT_MAX;

	d2d::Vector p = d2d::Math::rotateVector(d2d::Vector(-hWidth, -hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(-hWidth, hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(hWidth, -hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(hWidth, hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	wxPoint pos;
	pos.x = WIDTH * 0.5f * SCALE + sprite->getPosition().x * SCALE + xMin;
	pos.y = HEIGHT * 0.5f * SCALE - sprite->getPosition().y * SCALE + yMin;

	memDC.DrawBitmap(
		bitmap->ConvertToImage().Scale(width, height).Rotate(sprite->getAngle(), wxPoint(0, 0)),
		pos,
		true);
}

void SpriteDraw::drawSprite(const ISprite* sprite, const Vector& offset, wxMemoryDC& memDC,
							float bgwidth /*= 800*/, float bgheight /*= 480*/, float scale /*= 0.15f*/)
{
	if (!sprite->getSymbol().getBitmap()) return;

	const wxBitmap* bitmap = sprite->getSymbol().getBitmap()->getBitmap();

	const float width = std::max(1.0f, sprite->getSymbol().getWidth() * scale),
		height = std::max(1.0f, sprite->getSymbol().getHeight() * scale);
	const float hWidth = width * 0.5f,
		hHeight = height * 0.5f;

	float xMin, yMin;
	xMin = yMin = FLT_MAX;

	d2d::Vector p = d2d::Math::rotateVector(d2d::Vector(-hWidth, -hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(-hWidth, hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(hWidth, -hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	p = d2d::Math::rotateVector(d2d::Vector(hWidth, hHeight), sprite->getAngle());
	if (p.x < xMin) xMin = p.x;
	if (p.y < yMin) yMin = p.y;

	wxPoint pos;
	pos.x = bgwidth * 0.5f * scale + (sprite->getPosition().x + offset.x) * scale + xMin;
	pos.y = bgheight * 0.5f * scale - (sprite->getPosition().y + offset.y) * scale + yMin;

	memDC.DrawBitmap(
		bitmap->ConvertToImage().Scale(width, height).Rotate(sprite->getAngle(), wxPoint(0, 0)),
		pos,
		true);
}

void SpriteDraw::drawSprites(const std::vector<ISprite*>& sprites,
							 SpriteBatch& batch)
{
// 	GL10::Enable(GL10::GL_BLEND);
// 	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	batch.clear();
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
// 		ImageSprite* sprite = dynamic_cast<ImageSprite*>(sprites[i]);
// 		if (sprite)
// 			batch.add(sprite);
// 		else
			SpriteDraw::drawSprite(static_cast<ISprite*>(sprites[i]));
	}
	batch.onDraw();

	//GL10::Disable(GL10::GL_BLEND);
}

void SpriteDraw::begin(const ISprite* sprite)
{
	GL10::PushMatrix();

	GL10::Translatef(sprite->getPosition().x, sprite->getPosition().y, 0.0f);

	GL10::Rotatef(sprite->getAngle() * TRANS_RAD_TO_DEG, 0, 0, 1);

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	const float xScale = xMirror ? -sprite->getScaleX() : sprite->getScaleX(),
		yScale = yMirror ? -sprite->getScaleY() : sprite->getScaleY();
	GL10::Scalef(xScale, yScale, 1.0f);

}

void SpriteDraw::end(const ISprite* sprite)
{
	GL10::PopMatrix();
}

} // d2d