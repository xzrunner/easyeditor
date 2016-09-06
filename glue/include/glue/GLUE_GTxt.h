#ifndef _GLUE_GTXT_H_
#define _GLUE_GTXT_H_

#include <CU_Singleton.h>
#include <SM_Matrix.h>

#include <string>
#include <vector>

struct gtxt_label_style;

namespace s2 { class Color; }

namespace glue
{

class Color;

class GTxt
{
public:
	void Init(const std::vector<std::pair<std::string, std::string> >& fonts, 
		const std::vector<std::pair<std::string, std::string> >& user_fonts);

	void LoadFont(const std::string& name, const std::string& filepath);
	void LoadUserFont(const std::string& name, const std::string& filepath);

	void Draw(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
		const s2::Color& add, const std::string& text, int time, bool richtext) const;
	void Draw(const sm::mat4& mt, const std::string& str) const;

//	void Reload(const Sprite* spr);

	SINGLETON_DECLARATION(GTxt)

}; // GTxt
	
}

#endif // _GLUE_GTXT_H_