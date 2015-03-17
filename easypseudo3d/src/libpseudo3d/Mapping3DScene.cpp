#include "Mapping3DScene.h"
#include "StageCanvas3D.h"

#include <easy3d.h>
#include <easyimage.h>

#define STORE_2D

namespace epseudo3d
{

Mapping3DScene::Mapping3DScene(StageCanvas3D* canvas)
	: m_canvas(canvas)
	, m_img(NULL)
{
}

Mapping3DScene::~Mapping3DScene()
{
	delete m_img;
}

void Mapping3DScene::Store(const char* filename) const
{
	Json::Value value;

	// store vertices
	ivec2 min(INT_MAX, INT_MAX), max(-INT_MAX, -INT_MAX);
	for (int i = 0, n = m_vertices.size(); i < n; ++i) {
		ivec2 v = m_canvas->TransPos3ProjectToScreen(m_vertices[i]);

		value["vertices"][i]["x"] = v.x;
		value["vertices"][i]["y"] = v.y;

		if (v.x < min.x) min.x = v.x;
		if (v.x > max.x) max.x = v.x;
		if (v.y < min.y) min.y = v.y;
		if (v.y > max.y) max.y = v.y;
	}
	value["xmin"] = min.x;
	value["xmax"] = max.x;
	value["ymin"] = min.y;
	value["ymax"] = max.y;

	// store image
	int w = max.x - min.x,
		h = max.y - min.y;
	size_t size = w*h*4;
	unsigned char* pixels = new unsigned char[size];
	if (!pixels) {
		throw d2d::Exception("OOM");
	}
	memset(&pixels[0], 0, size);	

	int sw, sh;
	m_canvas->GetScreenSize(&sw, &sh);
	glReadPixels(min.x, sh - max.y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	eimage::ImageRevertY revert(pixels, w, h);
	uint8_t* pixels_revert = revert.Revert();
	delete[] pixels;

	std::string img_path = d2d::FilenameTools::getFilePathExceptExtension(filename);
	d2d::ImageSaver::storeToFile(pixels_revert, w, h, img_path, d2d::ImageSaver::e_png);
	delete[] pixels_revert;

	value["texture filepath"] = img_path + ".png";

	// store camera
	const e3d::Camera& cam = m_canvas->GetCamera3();
	const vec3& pos = cam.GetPosition();
	value["cam"]["x"] = pos.x;
	value["cam"]["y"] = pos.y;
	value["cam"]["z"] = pos.z;
	value["cam"]["rot_heading"] = cam.GetRotHeading();
	value["cam"]["rot_elevation"] = cam.GetRotElevation();
	
	// store to file
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void Mapping3DScene::Load(const char* filename)
{
	m_vertices.push_back(vec3(5, 5, 5));
	m_vertices.push_back(vec3(5, 5, -5));
	m_vertices.push_back(vec3(5, -5, 5));
	m_vertices.push_back(vec3(5, -5, -5));
	m_vertices.push_back(vec3(-5, 5, 5));
	m_vertices.push_back(vec3(-5, 5, -5));
	m_vertices.push_back(vec3(-5, -5, 5));
	m_vertices.push_back(vec3(-5, -5, -5));

#ifdef STORE_2D
 	m_img = d2d::ImageMgr::Instance()->getItem("box/crate.jpg");
 	return;
#endif // STORE_2D

	if (!filename) {
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string img_path = value["texture filepath"].asString();
	m_img = d2d::ImageMgr::Instance()->getItem(img_path);

	int xmin = value["xmin"].asInt(),
		xmax = value["xmax"].asInt(),
		ymin = value["ymin"].asInt(),
		ymax = value["ymax"].asInt();
	int w = xmax - xmin,
		h = ymax - ymin;

	m_texcoords.clear();
	int i = 0;
	Json::Value vertex_val = value["vertices"][i++];
	while (!vertex_val.isNull()) {
		int x = vertex_val["x"].asInt(),
			y = vertex_val["y"].asInt();
		vec2 texcoords;
		texcoords.x = (float)(x - xmin) / w;
		texcoords.y = 1 - (float)(y - ymin) / h;
		m_texcoords.push_back(texcoords);
		vertex_val = value["vertices"][i++];
	}

	e3d::Camera& cam = m_canvas->GetCamera3();
	vec3 pos;
	pos.x = value["cam"]["x"].asDouble();
	pos.y = value["cam"]["y"].asDouble();
	pos.z = value["cam"]["z"].asDouble();
	cam.SetPosition(pos);
	cam.SetRotHeading(value["cam"]["rot_heading"].asDouble());
	cam.SetRotElevation(value["cam"]["rot_elevation"].asDouble());
}

void Mapping3DScene::Draw() const
{
	if (!m_img) {
		return;
	}

#ifdef STORE_2D
 	e3d::DrawCube(vec3(-5, -5, -5), vec3(5, 5, 5), m_img->textureID());
 	return;

	//e3d::DrawCube(vec3(-5, -5, -5), vec3(5, 5, 5), d2d::WHITE);
#endif // STORE_2D

	std::vector<vec3> vertices;
	std::vector<vec2> texcoords;

	// front
	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[5]);
	texcoords.push_back(m_texcoords[5]);
	vertices.push_back(m_vertices[1]);
	texcoords.push_back(m_texcoords[1]);

	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[3]);
	texcoords.push_back(m_texcoords[3]);
	vertices.push_back(m_vertices[1]);
	texcoords.push_back(m_texcoords[1]);

	// back
	vertices.push_back(m_vertices[6]);
	texcoords.push_back(m_texcoords[6]);
	vertices.push_back(m_vertices[4]);
	texcoords.push_back(m_texcoords[4]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	vertices.push_back(m_vertices[6]);
	texcoords.push_back(m_texcoords[6]);
	vertices.push_back(m_vertices[2]);
	texcoords.push_back(m_texcoords[2]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	// left
	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[6]);
	texcoords.push_back(m_texcoords[6]);
	vertices.push_back(m_vertices[4]);
	texcoords.push_back(m_texcoords[4]);

	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[5]);
	texcoords.push_back(m_texcoords[5]);
	vertices.push_back(m_vertices[4]);
	texcoords.push_back(m_texcoords[4]);

	// right
	vertices.push_back(m_vertices[3]);
	texcoords.push_back(m_texcoords[3]);
	vertices.push_back(m_vertices[2]);
	texcoords.push_back(m_texcoords[2]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	vertices.push_back(m_vertices[3]);
	texcoords.push_back(m_texcoords[3]);
	vertices.push_back(m_vertices[1]);
	texcoords.push_back(m_texcoords[1]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	// bottom
	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[6]);
	texcoords.push_back(m_texcoords[6]);
	vertices.push_back(m_vertices[2]);
	texcoords.push_back(m_texcoords[2]);

	vertices.push_back(m_vertices[7]);
	texcoords.push_back(m_texcoords[7]);
	vertices.push_back(m_vertices[3]);
	texcoords.push_back(m_texcoords[3]);
	vertices.push_back(m_vertices[2]);
	texcoords.push_back(m_texcoords[2]);

	// top
	vertices.push_back(m_vertices[5]);
	texcoords.push_back(m_texcoords[5]);
	vertices.push_back(m_vertices[4]);
	texcoords.push_back(m_texcoords[4]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	vertices.push_back(m_vertices[5]);
	texcoords.push_back(m_texcoords[5]);
	vertices.push_back(m_vertices[1]);
	texcoords.push_back(m_texcoords[1]);
	vertices.push_back(m_vertices[0]);
	texcoords.push_back(m_texcoords[0]);

	e3d::ShaderMgr* shader = e3d::ShaderMgr::Instance();
	shader->Sprite();

	shader->DrawTri(&vertices[0].x, &texcoords[0].x, 2 * 6 * 3, m_img->textureID());
}

void Mapping3DScene::DebugDraw() const
{
// 	wxPaintDC dc((wxWindow*)m_canvas);
// 
// 	dc.SetPen(*wxWHITE_PEN);
// 	dc.SetBrush(*wxWHITE_BRUSH);
// 	for (int i = 0, n= m_vertices.size(); i < n; ++i) {
// 		ivec2 v = m_canvas->TransPos3ProjectToScreen(m_vertices[i]);
// 		dc.DrawRectangle(v.x, v.y, 10, 10);
// 	}
}

}