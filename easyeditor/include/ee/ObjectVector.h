#ifndef _EASYEDITOR_OBJECT_VECTOR_H_
#define _EASYEDITOR_OBJECT_VECTOR_H_

#include "DataTraverseType.h"
#include "Visitor.h"

#include <vector>

namespace ee
{

template<class T>
class ObjectVector
{
public:
	ObjectVector();
	virtual ~ObjectVector();
	
	void Traverse(RefVisitor<T>& visitor, bool order = true) const;
	void Traverse(RefVisitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Remove(const std::shared_ptr<T>& obj);

	bool Insert(const std::shared_ptr<T>& obj);
	bool Insert(const std::shared_ptr<T>& obj, int idx);

	bool Clear();

	bool ResetOrder(const std::shared_ptr<const T>& obj, bool up);
	bool ResetOrderMost(const std::shared_ptr<const T>& obj, bool up);
	bool Sort(std::vector<std::shared_ptr<T>>& list);

	bool IsExist(const std::shared_ptr<T>& obj) const;

	int Size() const;

	const std::vector<std::shared_ptr<T>>& GetObjs() const { return m_objs; }
	void SetObjs(const std::vector<std::shared_ptr<T>>& objs) { m_objs = objs; }

public:
	static void Traverse(const std::vector<std::shared_ptr<T>>& objs, RefVisitor<T>& visitor, bool order = true);
	static void Traverse(const std::vector<std::shared_ptr<T>>& objs, RefVisitor<T>& visitor, DataTraverseType type = DT_ALL, bool order = true);

	static bool Remove(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<T>& obj);

	static bool Insert(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<T>& obj);
	static bool Insert(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<T>& obj, int idx);

	static bool Clear(std::vector<std::shared_ptr<T>>& objs);

	static bool ResetOrder(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<const T>& obj, bool up);
	static bool ResetOrderMost(std::vector<std::shared_ptr<T>>& objs, const std::shared_ptr<const T>& obj, bool up);
	static bool Sort(std::vector<std::shared_ptr<T>>& objs, std::vector<std::shared_ptr<T>>& list);

private:
	std::vector<std::shared_ptr<T>> m_objs;

}; // ObjectVector

}

#include "ObjectVector.inl"

#endif // _EASYEDITOR_OBJECT_VECTOR_H_