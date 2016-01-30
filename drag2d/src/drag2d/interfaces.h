#ifndef _DRAG2D_INTERFACES_H_
#define _DRAG2D_INTERFACES_H_

#include <vector>

namespace d2d
{

class Sprite;
class EditPanel;
class ITask
{
public:
	virtual void Load(const char* filename) = 0;
	virtual void Store(const char* filename) const = 0;
	virtual bool IsDirty() const = 0;
	virtual void GetAllSprite(std::vector<const Sprite*>& sprites) const = 0;
	virtual const EditPanel* GetEditPanel() const = 0;
	virtual ~ITask() {}
};

class Object;
class IObjectStream
{
public:
	virtual Object* GetNext() = 0;
	virtual bool HasNext() const = 0;
	virtual void Rewind() = 0;
	virtual ~IObjectStream() {}
};

// 	class ISerializable
// 	{
// 	public:
// 		virtual void loadFromTextFile(std::ifstream& fin) = 0;
// 		virtual void storeToTextFile(std::ofstream& fout) const = 0;
// 		virtual ~ISerializable() {}
// 	}; // ISerializable

class Sprite;
class Vector;

}

#endif // _DRAG2D_INTERFACES_H_