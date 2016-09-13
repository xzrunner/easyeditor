#include "Package.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "Page.h"
#include "StaticAlloc.h"
#include "Allocator.h"
#include "simp_define.h"

#include <fs_file.h>
#include <fault.h>

#include <sstream>

#include <stddef.h>
#include <assert.h>

namespace simp
{

Package::Package(const std::string& filepath)
	: m_filepath(filepath)
{
	LoadIndex();
}

Package::~Package()
{
}

const void* Package::QueryNode(const std::string& name, int* type)
{
	*type = TYPE_INVALID;

	std::map<std::string, uint32_t>::iterator itr 
		= m_export_names.find(name);
	if (itr == m_export_names.end()) {
		return NULL;
	}

	uint32_t id = itr->second;
	Page* page = QueryPage(id);
	if (!page) {
		return NULL;
	}

	return page->Query(id, type);
}

void Package::LoadIndex()
{
	{
		std::string filepath = m_filepath + ".ept";
		m_images.clear();
		ImageDescLoader loader(filepath, m_images);
		loader.Load();
	}
	{
		std::string filepath = m_filepath + ".epe";
		m_export_names.clear();
		m_pages.clear();
		PageDescLoader loader(filepath, m_export_names, m_pages);
		loader.Load();
	}
}

Page* Package::QueryPage(int id)
{
	int idx = -1;
	int start = 0;
	int end = m_pages.size() - 1;
	while (start <= end)
	{
		int mid = (start + end) / 2;
		const PageDesc& p = m_pages[mid];
		if (id >= p.min && id <= p.max) {
			idx = mid;
			break;
		} else if (id < p.min) {
			end = mid - 1;
		} else {
			start = mid + 1;
		}
	}

	assert(idx != -1);
	if (!m_pages[idx].page) {
		LoadPage(idx);
	}

	return m_pages[idx].page;	
}

void Package::LoadPage(int idx)
{
	assert(!m_pages[idx].page);

	Allocator* alloc = StaticAlloc::Instance()->Create();

	int sz = ALIGN_4BYTE(Page::Size());
	void* ptr = alloc->Alloc(sz);
	Page* page = new (ptr) Page(alloc, m_pages[idx].min, m_pages[idx].max);

	std::ostringstream s;
	s << idx + 1;
 	std::string filepath = m_filepath + "." + s.str() + ".epe";
 	page->Load(filepath);
}

/************************************************************************/
/* class Package::ImageDescLoader                                       */
/************************************************************************/

Package::ImageDescLoader::ImageDescLoader(const std::string& filepath, std::vector<ImageDesc>& images) 
	: FileLoader(filepath)
	, m_images(images) 
{
}

void Package::ImageDescLoader::OnLoad(ImportStream& is)
{
	int num = is.UInt32();
	m_images.reserve(num);
	for (int i = 0; i < num; ++i)
	{
		ImageDesc img;
		img.w = is.UInt16();
		img.h = is.UInt16();
		img.type = is.UInt16();
		m_images.push_back(img);
	}
}

/************************************************************************/
/* class Package::PageDescLoader                                        */
/************************************************************************/

Package::PageDescLoader::PageDescLoader(const std::string& filepath, 
									  std::map<std::string, uint32_t>& export_names,
									  std::vector<PageDesc>& pages)
	: FileLoader(filepath)
	, m_export_names(export_names)
	, m_pages(pages)
{
}

void Package::PageDescLoader::OnLoad(ImportStream& is)
{
	int export_n = is.UInt16();
	for (int i = 0; i < export_n; ++i)
	{
		std::string name = is.String();
		uint32_t id = is.UInt32();
		m_export_names.insert(std::make_pair(name, id));
	}

	int page_n = is.UInt16();
	m_pages.reserve(page_n);
	for (int i = 0; i < page_n; ++i)
	{
		PageDesc page;
		page.min = is.UInt32();
		page.max = is.UInt32();
		m_pages.push_back(page);
	}
}

}