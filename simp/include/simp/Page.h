#ifndef _SIMP_PAGE_H_
#define _SIMP_PAGE_H_

#include "FileLoader.h"

#include <CU_Uncopyable.h>

#include <string>

#include <stdint.h>

namespace simp
{

class Allocator;
class ImportStream;

class Page : private cu::Uncopyable
{
public:
	Page(Allocator* alloc, int begin_id, int end_id);
	~Page();

	void Load(const std::string& filepath);

	const void* Query(uint32_t id, int* type) const;

	static int Size();

private:
	class Loader : public FileLoader
	{
	public:
		Loader(const std::string& filepath, Page* page);

	protected:
		virtual void OnLoad(ImportStream& is);

	private:
		static void* CreateNode(uint8_t type, Allocator& alloc, ImportStream& is);

	private:
		Page* m_page;

	}; // Loader

private:
	Allocator* m_alloc;

	int m_begin_id, m_end_id;	

	uint8_t* m_types;
	void** m_nodes;

}; // Page

}

#endif // _SIMP_PAGE_H_