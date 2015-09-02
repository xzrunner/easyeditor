#ifndef _EPBIN_EPD_DATASET_H_
#define _EPBIN_EPD_DATASET_H_

#include "../common_dataset.h"

#include <vector>
#include <set>

struct lua_State;

namespace epbin
{
namespace epd
{

class Picture
{
public:
	Picture(lua_State* L, int id);

	size_t Size() const;

	void Store(uint8_t** ptr);

private:
	struct Part
	{
		uint8_t tex;
		uint16_t src[8];
		int32_t screen[8];

		size_t Size() const;

		void Store(uint8_t** ptr);
	};

private:
	uint16_t m_id;

	std::vector<Part> m_parts;

}; // Picture

class IPackNode
{
public:
	IPackNode(uint8_t type) : m_type(type) {}

	virtual size_t Size() const = 0;
	virtual void Store(uint8_t** ptr) const = 0;

	uint8_t GetType() const { return m_type; }

private:
	uint8_t m_type;

}; // IPackNode

class Component : public IPackNode
{
public:
	Component(lua_State* L);

	virtual size_t Size() const;
	virtual void Store(uint8_t** ptr) const;

private:
	uint16_t m_id;

}; // Component

class Switch : public IPackNode
{
public:
	Switch(lua_State* L);

	virtual size_t Size() const;
	virtual void Store(uint8_t** ptr) const;

private:
	uint16_t m_id;
	String m_name;

}; // Switch

class Label : public IPackNode
{
public:
	Label(lua_State* L);

	virtual size_t Size() const;
	virtual void Store(uint8_t** ptr) const;

private:
	String m_name;
	String m_font;
	uint32_t m_color;
	uint8_t m_size;
	uint8_t m_align;
	uint16_t m_width, m_height;

}; // Label

class Mount : public IPackNode
{
public:
	Mount(lua_State* L);

	virtual size_t Size() const;
	virtual void Store(uint8_t** ptr) const;

private:
	String m_name;

}; // Mount

class Sprite
{
public:
	Sprite(lua_State* L, int max_idx);
	~Sprite();

	virtual size_t Size() const;
	void Store(uint8_t** ptr) const;

private:
	struct Matrix
	{
		int32_t m[6];

		Matrix();
	};

private:
	uint8_t m_type;
	uint16_t m_id;

	uint32_t* m_color;
	uint32_t* m_add;

	Matrix* m_mat;

}; // Sprite

class Frame
{
public:
	~Frame();

	size_t Size() const;

	void Store(uint8_t** ptr);

public:
	std::vector<Sprite*> sprites;

}; // Frame

class Action
{
public:
	~Action();

	size_t Size() const;

	void Store(uint8_t** ptr);

public:
	String action;
	std::vector<Frame*> frames;

}; // Action

class Animation
{
public:
	Animation(lua_State* L, int id);
	~Animation();

	size_t Size() const;

	void Store(uint8_t** ptr);

private:
	struct Clipbox
	{
		int32_t cb[4];

		size_t Size() const { return sizeof(cb); }
	};

private:
	uint8_t m_type;
	uint16_t m_id;

	Clipbox* m_clipbox;

	String m_export_name;

	std::vector<IPackNode*> m_components;

	std::vector<Action*> m_actions;

}; // Animation

}
}

#endif // _EPBIN_EPD_DATASET_H_
