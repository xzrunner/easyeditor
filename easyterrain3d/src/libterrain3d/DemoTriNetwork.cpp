#include "DemoTriNetwork.h"

#include "randtab.h"

namespace libterrain3d
{

static const float EDGE = 10;

static const int MAX_LEVEL = 10;

DemoTriNetwork::DemoTriNetwork(e3d::Camera& cam)
	: m_cam(cam)
	, m_tex(0)
{
}

void DemoTriNetwork::Load()
{
	m_cam.SetPosition(vec3(0, -8, 8));
	m_cam.Rotate(0, 40);

	GenTex();

	m_level2size.reserve(MAX_LEVEL + 1);
	for(int i = 0; i <= MAX_LEVEL; ++i) {
		m_level2size.push_back(3.0f / ((float)sqrt((float)(1 << i))));
	}
}

void DemoTriNetwork::Draw() const
{
	const float HALF_EDGE = EDGE * 0.5f;
	vec3 v0(-HALF_EDGE,  HALF_EDGE, 0), 
		 v1( HALF_EDGE,  HALF_EDGE, 0),
		 v2( HALF_EDGE, -HALF_EDGE, 0),
		 v3(-HALF_EDGE, -HALF_EDGE, 0);
	DrawTriByLevel(v1, v0, v2, 0);
	DrawTriByLevel(v3, v0, v2, 0);
}

void DemoTriNetwork::DrawTriByLevel(const vec3& v0, const vec3& v1, const vec3& v2, int level) const
{
	vec3 c = (v0 + v1 + v2) / 3;
	float dis = e3d::Math3::GetDistanceSquare(c, m_cam.GetPosition());
	float len = e3d::Math3::GetDistanceSquare(v1, v2);
//	e3d::LogViewer::Instance()->Add(("dis:"+wxString::FromDouble(dis)).ToStdString());

	if (level < MAX_LEVEL && len > dis * 0.005f)
	{
		vec3 new_node = (v1 + v2) * 0.5f;
		new_node.z = GetRandHeight(v1, v2, level);
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

void DemoTriNetwork::GenTex()
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

float DemoTriNetwork::GetRandHeight(const vec3& v1, const vec3& v2, int level) const
{
	float rand_height;

	float src[2] = {
		(v1.x + v2.x) * 0.5f,
		0.0f
	};

	//determine random perturbation of center z using hash of x,y
	//random number lookup per byte of (x, z) data, all added
	unsigned char* pC = (unsigned char*)src;
	unsigned int uiS = 0;
	for(int i = 0; i < 8; ++i) {
		uiS += randtab[(i << 8) | pC[i]];
	}

	//stuff random hash value bits from uiS into float (float viewed
	//as an int, IEEE float tricks here...)
	int* pInt = (int*)(&rand_height);

	*pInt	    = 0x40000000 + (uiS & 0x007fffff);
	rand_height-= 3.0f;
	rand_height	= rand_height * m_level2size[level];

	return (v1.z + v2.z) * 0.5f + rand_height;
}

}