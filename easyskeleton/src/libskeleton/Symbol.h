#ifndef _LIBSKELETON_SYMBOL_H_
#define _LIBSKELETON_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/SkeletonSymbol.h>
#include <gum/JointLoader.h>

namespace s2 { class Sprite; class Joint; }

namespace libskeleton
{

class Symbol : public ee::Symbol, public s2::SkeletonSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	void DrawSkeleton(const s2::RenderParams& params, const s2::Sprite* spr = NULL, 
		const std::shared_ptr<const s2::Joint>& selected = NULL) const;

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

private:
	class JointLoader : public gum::JointLoader
	{
	public:
		virtual std::shared_ptr<s2::Joint> Create(const s2::SprPtr& spr, const s2::JointPose& joint_pose) const override;
	}; // JointLoader

protected:
	virtual bool LoadResources() override;

}; // Symbol

}

#endif // _LIBSKELETON_SYMBOL_H_
