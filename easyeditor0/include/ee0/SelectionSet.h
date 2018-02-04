#pragma once

#include <memory>
#include <functional>
#include <vector>

namespace ee0
{

template<class T>
class SelectionSet
{
public:
	virtual ~SelectionSet() {}

	virtual void Clear();

	virtual void Add(const std::shared_ptr<T>& item);
	virtual void Remove(const std::shared_ptr<T>& item);

	size_t Size() const;
	bool IsEmpty() const;

	bool IsExist(const std::shared_ptr<T>& item) const;

	void Traverse(std::function<bool(const std::shared_ptr<T>&)> func) const;

protected:
	std::vector<std::shared_ptr<T>> m_items;

}; // SelectionSet

}

#include "ee0/SelectionSet.inl"