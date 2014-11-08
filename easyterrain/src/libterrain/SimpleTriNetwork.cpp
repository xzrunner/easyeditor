#include "SimpleTriNetwork.h"

namespace libterrain
{

static const float EDGE = 10;

static const int MAX_LEVEL = 10;

SimpleTriNetwork::SimpleTriNetwork(e3d::Camera& cam)
	: m_cam(cam)
	, m_tex(0)
{
	m_cam.SetPosition(vec3(0, -2, 2));
	m_cam.Rotate(0, 30);
}

void SimpleTriNetwork::Load()
{
	GenTex();
}

void SimpleTriNetwork::Draw() const
{
	const float HALF_EDGE = EDGE * 0.5f;
	vec3 v0(-HALF_EDGE,  HALF_EDGE, 0), 
		 v1( HALF_EDGE,  HALF_EDGE, 0),
		 v2( HALF_EDGE, -HALF_EDGE, 0),
		 v3(-HALF_EDGE, -HALF_EDGE, 0);
	DrawTriByLevel(v1, v0, v2, 0);
	DrawTriByLevel(v3, v0, v2, 0);
}

void SimpleTriNetwork::DrawTriByLevel(const vec3& v0, const vec3& v1, const vec3& v2, int level) const
{
	vec3 c = (v0 + v1 + v2) / 3;
	float dis = e3d::Math3::GetDistance(c, m_cam.GetPosition());
//	e3d::LogViewer::Instance()->Add(("dis:"+wxString::FromDouble(dis)).ToStdString());

	if (level < MAX_LEVEL && dis < (MAX_LEVEL - level))
	{
		vec3 new_node = (v1 + v2) * 0.5f;
		DrawTriByLevel(new_node, v0, v1, level + 1);
		DrawTriByLevel(new_node, v0, v2, level + 1);
		return;
	}

//	e3d::DrawTriLine(v0, v1, v2);

 	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();
 	shader->Sprite();
 	vec3 vertices[] = {v0, v1, v2};
  	vec2 texcoords[] = {
		vec2(1, 0),
  		vec2(0, 0),
  		vec2(1, 1),
  	};
 	shader->DrawTri(vertices, texcoords, m_tex);
}

void SimpleTriNetwork::GenTex()
{
	//generate pixels
	const int SIZE = 128;
	byte* pixels = new byte[SIZE * SIZE * 4];
	for(int y = 0; y < SIZE; ++y)
	{
		for(int x = 0; x < SIZE; ++x)
		{
			//create bump-shaped function f that is zero on each edge
			float a0 = (float)y / 127.0f;
			float a1 = (float)(127 - x) / 127.0f;
			float a2 = (float)(x - y) / 127.0f;
			float f = (float)sqrt(a0*a1*a2);

			//quantize the function value and make into color
			//store in rgb components of texture entry
			int k = (int)floor(1400.0f * f) ;
			if(k < 0) {
				k = 0;
			}
			if (k > 255) {
				k = 255;
			}

			pixels[4*(y*SIZE+x)+0] = 30 + (sqrt((float)k)) / 290.0f;
			pixels[4*(y*SIZE+x)+1] = k;
			pixels[4*(y*SIZE+x)+2] = (k < 128 ? 0 : (k-128)*2);
			pixels[4*(y*SIZE+x)+3] = 0;
		}
	}

	//set up the gridview texture
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SIZE, SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SIZE, SIZE, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

}