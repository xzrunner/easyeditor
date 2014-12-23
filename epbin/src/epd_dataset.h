#ifndef _EPBIN_EPD_DATASET_H_
#define _EPBIN_EPD_DATASET_H_

#include "typedef.h"

#include <vector>

namespace epbin
{

class String
{
public:
	String(const std::string& str, bool is_empty = false);

	size_t Size() const;

private:
	bool m_is_empty;

	std::string m_str;

}; // String

class Picture
{
public:
	size_t Size() const;

public:
	struct Part
	{
		uint8_t tex;
		uint16_t src[8];
		int32_t screen[8];
	};

public:
	uint16_t m_id;

	std::vector<Part> m_parts;

}; // Picture

class IComponent
{
public:
	IComponent(uint8_t type) : m_type(type) {}

	virtual size_t Size() const = 0;

protected:
	uint8_t m_type;

}; // IComponent

class Component : public IComponent
{
public:
	Component();

	virtual size_t Size() const;

private:
	uint16_t m_id;

}; // Component

class Switch : public IComponent
{
public:
	Switch(const std::string& name);

	virtual size_t Size() const;

private:
	uint16_t m_id;
	String m_name;

}; // Switch

class Label : public IComponent
{
public:
	Label(const std::string& name, const std::string& font);

	virtual size_t Size() const;

private:
	String m_name;
	String m_font;
	uint32_t m_color;
	uint8_t m_size;
	uint8_t m_align;
	uint16_t m_width, m_height;

}; // Label

class Mount : public IComponent
{
public:
	Mount(const std::string& name);

	virtual size_t Size() const;

private:
	String m_name;

}; // Mount

struct Matrix
{
	int32_t m[6];
};

class Frame
{
public:
	Frame();
	~Frame();

	virtual size_t Size() const;

private:
	uint8_t m_type;
	uint16_t m_id;

	uint32_t* m_color;
	uint32_t* m_add;

	Matrix* m_mat;

}; // Frame

class Animation
{
public:
	Animation();
	~Animation();

private:
	uint8_t m_type;
	uint16_t m_id;

	int32_t m_clipbox[4];

	String m_export_name;

	std::vector<IComponent*> m_components;

	std::vector<Frame*> m_frames;

}; // Animation

}

#endif // _EPBIN_EPD_DATASET_H_
