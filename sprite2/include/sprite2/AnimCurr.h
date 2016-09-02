#ifndef _SPRITE2_ANIM_CURR_H_
#define _SPRITE2_ANIM_CURR_H_

#include <vector>

namespace s2
{

class Sprite;
class AnimSymbol;
class RenderParams;

class AnimCurr
{
public:
	AnimCurr();
	AnimCurr(const AnimCurr& curr);
	AnimCurr& operator = (const AnimCurr& curr);
	AnimCurr(AnimSymbol* sym);
	~AnimCurr();

	bool Update(const RenderParams& params, bool loop = true, 
		float interval = 0, int fps = 30);
	void Draw(const RenderParams& params) const;

	Sprite* FetchChild(const std::string& name) const;

	void Start();

	void Clear();

private:
	void LoadFromSym();

private:
	class Frame
	{
	public:
		Frame();
 		Frame(const Frame& f);
 		Frame& operator = (const Frame& f);
		~Frame();

		Sprite* Query(const Sprite* spr, int idx);

		void Clear();

	private:
		void CopyFrom(const std::vector<Sprite*>& src);

	public:
		std::vector<Sprite*> sprs;		

	}; // Frame

	class Layer
	{
	public:
		Frame frame;
	}; // Layer

private:
	AnimSymbol* m_sym;

	int m_frame;

	std::vector<Layer> m_layers;

	float m_start_time, m_curr_time;

}; // AnimCurr

}

#endif // _SPRITE2_ANIM_CURR_H_