#include "SpriteBatch.h"

#include "dataset/ImageSprite.h"

#include <gl/glew.h>

namespace d2d
{

SpriteBatch::SpriteBatch(int size, int usage)
	: m_textureID(0)
	, m_size(size)
	, m_next(0)
	, m_drawTimes(0)
	, m_vertices(NULL)
	, m_indices(NULL)
{
	createBuffer(size, usage);
	fillIndicesBuffer(size);
}

SpriteBatch::~SpriteBatch()
{
	delete m_vertices;
	delete m_indices;
}

int SpriteBatch::add(const ImageSprite* sprite, int index/* = -1*/)
{
	if (index < -1 || index >= m_next)
		return -1;

	const ImageSymbol& symbol = sprite->getSymbol();

	if ((index == -1 && m_next >= m_size)
		|| (m_next != 0 && m_textureID != symbol.getTextureID()))
	{
		onDraw();
		m_next = 0;
	}

	if (m_next == 0)
	{
		m_textureID = symbol.getTextureID();

// 		const float hw = symbol.getWidth() * 0.5f,
// 			hh = symbol.getHeight() * 0.5f;
// 		m_node[0] = -hw;
// 		m_node[1] = -hh;
// 		m_node[2] = -hw;
// 		m_node[3] = hh;
// 		m_node[4] = hw;
// 		m_node[5] = hh;
	}

	const float hw = symbol.getWidth() * sprite->getScaleX() * 0.5f,
		hh = symbol.getHeight() * sprite->getScaleY() * 0.5f;
	float x0 = -hw;
	float y0 = -hh;
	float x1 = -hw;
	float y1 = hh;
	float x2 = hw;
	float y2 = hh;

	const float sine = sin(sprite->getAngle()),
		cosine = cos(sprite->getAngle());
	const float x = sprite->getPosition().x,
		y = sprite->getPosition().y;

	m_sprite[0].x = cosine * x0 - sine * y0 + x;
	m_sprite[0].y = sine * x0 + cosine * y0 + y;

	m_sprite[1].x = cosine * x1 - sine * y1 + x;
	m_sprite[1].y = sine * x1 + cosine * y1 + y;

	m_sprite[2].x = cosine * x2 - sine * y2 + x;
	m_sprite[2].y = sine * x2 + cosine * y2 + y;

	m_sprite[3].x = m_sprite[0].x + (m_sprite[2].x - m_sprite[1].x);
	m_sprite[3].y = m_sprite[2].y - (m_sprite[1].y - m_sprite[0].y);

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	if (xMirror)
	{
		m_sprite[0].u = 1;
		m_sprite[1].u = 1;
		m_sprite[2].u = 0;
		m_sprite[3].u = 0;
	}
	else
	{
		m_sprite[0].u = 0;
		m_sprite[1].u = 0;
		m_sprite[2].u = 1;
		m_sprite[3].u = 1;
	}
	if (yMirror)
	{
		m_sprite[0].v = 1;
		m_sprite[1].v = 0;
		m_sprite[2].v = 0;
		m_sprite[3].v = 1;
	}
	else
	{
		m_sprite[0].v = 0;
		m_sprite[1].v = 1;
		m_sprite[2].v = 1;
		m_sprite[3].v = 0;
	}

	add(m_sprite, (index == -1 ? m_next : index));

	if (index == -1)
		return m_next++;
	return index;
}


void* SpriteBatch::lock()
{
	m_vertices->bind();
	void* ret = m_vertices->map();
	m_vertices->unbind();

	return ret;
}

void SpriteBatch::unlock()
{
	m_vertices->bind();
	m_vertices->unmap();
	m_vertices->unbind();
}

void SpriteBatch::onDraw() const
{
	if (!m_textureID) return;

	++m_drawTimes;

	glColor4f(1, 1, 1, 1);

	const int color_offset = 0;
	const int vertex_offset = sizeof(unsigned char) * 4;
	const int texel_offset = sizeof(unsigned char) * 4 + sizeof(float) * 2;

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, m_textureID);

	m_vertices->bind();
	m_indices->bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), m_vertices->getPointer(vertex_offset));

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), m_vertices->getPointer(texel_offset));

	glDrawElements(GL_TRIANGLES, m_next * 6, GL_UNSIGNED_SHORT, m_indices->getPointer(0));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	m_indices->unbind();
	m_vertices->unbind();

	glBindTexture(GL_TEXTURE_2D, NULL);

	glPopMatrix();
}

void SpriteBatch::createBuffer(int size, int usage)
{
	GLenum gl_usage;
	switch (usage)
	{
	default:
	case USAGE_DYNAMIC:
		gl_usage = GL_DYNAMIC_DRAW;
		break;
	case USAGE_STATIC:
		gl_usage = GL_STATIC_DRAW;
		break;
		// 	case USAGE_STREAM:
		// 		gl_usage = GL_STREAM_DRAW;
		// 		break;
	}

	int vertex_size = sizeof(Vertex) * 4 * size;
	int index_size = sizeof(GLushort) * 6 * size;

	m_vertices = VertexBuffer::create(vertex_size, GL_ARRAY_BUFFER, gl_usage);
	m_indices = VertexBuffer::create(index_size, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

void SpriteBatch::fillIndicesBuffer(int size)
{
	m_indices->bind();
	GLushort* indices = static_cast<GLushort*>(m_indices->map());

	if (indices)
	{
		for (int i = 0; i < size; ++i)
		{
			indices[i*6+0] = 0+(i*4);
			indices[i*6+1] = 1+(i*4);
			indices[i*6+2] = 2+(i*4);

			indices[i*6+3] = 0+(i*4);
			indices[i*6+4] = 2+(i*4);
			indices[i*6+5] = 3+(i*4);
		}
	}

	m_indices->unmap();
	m_indices->unbind();
}

void SpriteBatch::add(const Vertex* v, int index)
{
	int sprite_size = sizeof(Vertex) * 4;

	m_vertices->bind();
	m_vertices->fill(index * sprite_size, sprite_size, v);
	m_vertices->unbind();
}

// void SpriteBatch::draw(const std::vector<ISprite*>& sprites) const
// {
// 	std::vector<Vector> vertices;
// 	std::vector<Vector> texCoords;
// 	ImageSymbol* last = NULL;
// 	unsigned int id = 0;
// 	int width, height;
// 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
// 	{
// 		ImageSprite* sprite = dynamic_cast<ImageSprite*>(sprites[i]);
// 		if (!sprite)
// 		{
// 			drawSprite(sprites[i]);
// 			continue;
// 		}
// 
// 		//		const ImageSymbol* curr = &dynamic_cast<ImageSymbol&>(const_cast<ISymbol&>(sprite->getSymbol()));
// 		const ImageSymbol* curr = &sprite->getSymbol();
// 
// 		if (curr != last)
// 		{
// 			if (!vertices.empty())
// 			{
// 				PrimitiveDraw::drawMesh(id, vertices, texCoords);
// 				vertices.clear();
// 				texCoords.clear();
// 			}
// 
// 			last = const_cast<ImageSymbol*>(curr);
// 
// 			id = last->getTextureID();
// 			width = last->getWidth();
// 			height = last->getHeight();
// 		}
// 
// 		Vector quad[4];
// 		Math::computeQuadNodes(sprite->getPosition(), sprite->getAngle(), 
// 			sprite->getScale(), width, height, quad);
// 
// 		vertices.push_back(quad[0]);
// 		vertices.push_back(quad[3]);
// 		vertices.push_back(quad[2]);
// 
// 		vertices.push_back(quad[0]);
// 		vertices.push_back(quad[2]);
// 		vertices.push_back(quad[1]);
// 
// 		bool xMirror, yMirror;
// 		sprite->getMirror(xMirror, yMirror);
// 
// 		Vector leftLow(0, 0), rightLow(1, 0),
// 			leftTop(0, 1), rightTop(1, 1);
// 		if (xMirror)
// 		{
// 			leftLow.x = 1;
// 			rightLow.x = 0;
// 			leftTop.x = 1;
// 			rightTop.x = 0;
// 		}
// 		if (yMirror)
// 		{
// 			leftLow.y = 1;
// 			rightLow.y = 1;
// 			leftTop.y = 0;
// 			rightTop.y = 0;
// 		}
// 
// 		texCoords.push_back(Vector(leftLow.x, leftLow.y));
// 		texCoords.push_back(Vector(rightLow.x, rightLow.y));
// 		texCoords.push_back(Vector(rightTop.x, rightTop.y));
// 
// 		texCoords.push_back(Vector(leftLow.x, leftLow.y));
// 		texCoords.push_back(Vector(rightTop.x, rightTop.y));
// 		texCoords.push_back(Vector(leftTop.x, leftTop.y));
// 	}
// 
// 	if (last && !vertices.empty())
// 		PrimitiveDraw::drawMesh(id, vertices, texCoords);
// }

} // d2d