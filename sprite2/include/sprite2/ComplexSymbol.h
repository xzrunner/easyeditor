#ifndef _SPRITE2_COMPLEX_SYMBOL_H_
#define _SPRITE2_COMPLEX_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class ComplexSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ComplexSymbol();
	virtual ~ComplexSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const std::vector<Sprite*>& GetChildren() const { return m_children; }
	
	const sm::rect& GetScissor() const { return m_scissor; }
	void SetScissor(const sm::rect& scissor) { m_scissor = scissor; }

	int GetActionIdx(const std::string& name) const;

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	// todo up action
	bool Add(Sprite* spr, int idx = -1);
	bool Remove(Sprite* spr);
	bool Clear();
	bool ResetOrder(const Sprite* spr, bool up);
	bool ResetOrderMost(const Sprite* spr, bool up);
	bool Sort(std::vector<Sprite*>& sprs);

protected:
	const std::vector<Sprite*>& GetSprs(int action) const;

public:
	struct Action
	{
		std::string name;
		std::vector<Sprite*> sprs;
	};

	const std::vector<Action>& GetActions() const { return m_actions; }
	void SetActions(const std::vector<Action>& actions) { m_actions = actions; }

protected:
	std::vector<Sprite*> m_children;

	std::vector<Action> m_actions;

	sm::rect m_scissor;

}; // ComplexSymbol

}

#endif // _SPRITE2_COMPLEX_SYMBOL_H_