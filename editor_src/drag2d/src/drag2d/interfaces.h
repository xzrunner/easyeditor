#pragma once

#include <fstream>

namespace d2d
{
	class ITask
	{
	public:
		virtual void load(const char* filename) = 0;
		virtual void store(const char* filename) const = 0;
		virtual bool isDirty() const = 0;
		virtual void clear() = 0;
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

	class ISerializable
	{
	public:
		virtual void loadFromTextFile(std::ifstream& fin) = 0;
		virtual void storeToTextFile(std::ofstream& fout) const = 0;
		virtual ~ISerializable() {}
	}; // ISerializable

	class ICloneable 
	{
	public:
		virtual ICloneable* clone() = 0;
		virtual ~ICloneable() {}
	}; // ICloneable

	class ICameraObserver
	{
	public:
		virtual void onCameraChanged() = 0;
		virtual ~ICameraObserver() {}
	}; // ICameraObserver
}