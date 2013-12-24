#pragma once

#include <wx/wx.h>
#include <vector>

#include "interfaces.h"

namespace d2d
{
	template <typename T>
	class FetchAllVisitor : public IVisitor
	{
	public:
		FetchAllVisitor(std::vector<T*>& result) 
			: m_result(result) {}

		virtual void visit(Object* object, bool& bFetchNext)
		{
			T* item = dynamic_cast<T*>(object);
			if (item)
				m_result.push_back(item);
			bFetchNext = true;
		}

	private:
		std::vector<T*>& m_result;

	}; // FetchAllVisitor

	//	template <typename T>
	class CountVerifyVisitor : public IVisitor
	{
	public:
		CountVerifyVisitor(bool& valid, int num)
			: m_valid(valid), m_num(num), m_count(0) {}

		virtual ~CountVerifyVisitor() {
			m_valid = m_count == m_num;
		}

		void visit(Object* object, bool& bFetchNext) {
			++m_count;
			bFetchNext = m_count <= m_num;
		}

	private:
		bool& m_valid;
		const int m_num;

		int m_count;

	}; // CountVerifyVisitor

	class EmptyVerifyVisitor : public IVisitor
	{
	public:
		EmptyVerifyVisitor(bool& valid) 
			: m_valid(valid) { m_valid = true; }

		void visit(Object* object, bool& bFetchNext) {
			m_valid = false;
			bFetchNext = false;
		}

	private:
		bool& m_valid;

	}; // EmptyVerifyVisitor

	template <typename T>
	class GetNameVisitor : public IVisitor
	{
	public:
		GetNameVisitor(std::vector<wxString>& names)
			: m_names(names) {}

		virtual void visit(Object* object, bool& bFetchNext)
		{
			T* item = dynamic_cast<T*>(object);
			if (item)
				m_names.push_back(item->getName());
			bFetchNext = true;
		}

	private:
		std::vector<wxString>& m_names;

	}; // GetNameVisitor
}

