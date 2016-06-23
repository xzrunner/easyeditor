#ifndef _EASYEDITOR_GTXT_H_
#define _EASYEDITOR_GTXT_H_

#include <SM_Matrix.h>
#include <sprite2/Color.h>

#include <string>

struct gtxt_label_style;

namespace ee
{

class GTxt
{
public:
	void LoadFont(const char* filepath);

 	void Draw(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
		const s2::Color& add, const std::string& text, int time, bool richtext) const;
	void Draw(const sm::mat4& mt, const std::string& str) const;

//	void Reload(const Sprite* spr);

	static GTxt* Instance();

private:
	GTxt();

	void Init();

private:
	static GTxt* m_instance;

}; // GTxt

}

#endif // _EASYEDITOR_GTXT_H_