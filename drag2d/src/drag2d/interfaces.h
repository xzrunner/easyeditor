#pragma once

#include <fstream>
#include <vector>

namespace d2d
{
	class ISprite;
	class EditPanel;
	class ITask
	{
	public:
		virtual void load(const char* filename) = 0;
		virtual void store(const char* filename) const = 0;
		virtual bool isDirty() const = 0;
		virtual void clear() = 0;
		virtual void getAllSprite(std::vector<const ISprite*>& sprites) const = 0;
		virtual const EditPanel* getEditPanel() const = 0;
		virtual ~ITask() {}
	};

	class Object;
	class IObjectStream
	{
	public:
		virtual Object* getNext() = 0;
		virtual bool hasNext() const = 0;
//		virtual size_t size() = 0;
		virtual void rewind() = 0;
		virtual ~IObjectStream() {}
	};

	class IVisitor
	{
	public:
		virtual void visit(Object* object, bool& bFetchNext) = 0;
		virtual ~IVisitor() {}
	}; // IVisitor

// 	class ISerializable
// 	{
// 	public:
// 		virtual void loadFromTextFile(std::ifstream& fin) = 0;
// 		virtual void storeToTextFile(std::ofstream& fout) const = 0;
// 		virtual ~ISerializable() {}
// 	}; // ISerializable

	class ICloneable 
	{
	public:
		virtual ICloneable* clone() const = 0;
		virtual ~ICloneable() {}
	}; // ICloneable

	class ICameraObserver
	{
	public:
		virtual void onCameraChanged() = 0;
		virtual ~ICameraObserver() {}
	}; // ICameraObserver

	class ISprite;
	class Vector;

	class ISpriteObserver
	{
	public:
		virtual void translate(ISprite* sprite, const Vector& offset) = 0;
		virtual void rotate(ISprite* sprite, float delta) = 0;
		virtual ~ISpriteObserver() {}
	}; // ISpriteObserver
}