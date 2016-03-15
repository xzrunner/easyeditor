#ifndef _GLUE_SYMBOL_H_
#define _GLUE_SYMBOL_H_

#include <string>

#include "math/Matrix.h"

namespace glue
{

class Texture;

class Symbol
{
public:
	Symbol(const std::string& filepath);

	virtual void Draw(const mat4& mt) const;

	// debug
	void ModelDraw(const mat4& mt) const;

private:
	void Load();

private:
	std::string m_filepath;

	Texture* m_tex;

}; // Symbol

}

#endif // _GLUE_SYMBOL_H_