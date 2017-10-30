#ifndef _EASYEDITOR_FETCH_ALL_VISITOR_H_
#define _EASYEDITOR_FETCH_ALL_VISITOR_H_

#include "Visitor.h"

namespace ee
{

template <typename T>
class FetchAllVisitor : public Visitor<T>
{
public:
	FetchAllVisitor(std::vector<T*>& result) 
		: m_result(result) {}

	virtual void Visit(T* obj, bool& next)
	{
		if (obj) {
			m_result.push_back(obj);
		}
		next = true;
	}

private:
	std::vector<T*>& m_result;

}; // FetchAllVisitor

template <typename T>
class FetchAllRefVisitor : public RefVisitor<T>
{
public:
	FetchAllRefVisitor(std::vector<std::shared_ptr<T>>& result)
		: m_result(result) {}

	virtual void Visit(const std::shared_ptr<T>& obj, bool& next)
	{
		if (obj) {
			m_result.push_back(obj);
		}
		next = true;
	}

private:
	std::vector<std::shared_ptr<T>>& m_result;

}; // FetchAllRefVisitor

}

#endif // _EASYEDITOR_FETCH_ALL_VISITOR_H_