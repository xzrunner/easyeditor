#ifndef _SIMP_PACKAGE_H_
#define _SIMP_PACKAGE_H_

#include "FileLoader.h"

#include <CU_Uncopyable.h>

#include <string>
#include <vector>
#include <map>

#include <stdint.h>

namespace simp
{

class Image;
class Page;

class Package : private cu::Uncopyable
{
public:
	Package(const std::string& filepath);
	~Package();

	uint32_t QueryID(const std::string& name) const;

	const void* QueryNode(uint32_t id, int* type);

	std::string GetImagePath(int idx) const;

private:
	void LoadIndex();

	Page* QueryPage(int id);
	void LoadPage(int idx);

private:
	/************************************************************************/
	/* image desc                                                           */
	/************************************************************************/

	struct ImageDesc
	{
		int w, h;
		int type;
	};

	class ImageDescLoader : public FileLoader
	{
	public:
		ImageDescLoader(const std::string& filepath, std::vector<ImageDesc>& images);

	protected:
		virtual void OnLoad(ImportStream& is);

	private:
		std::vector<ImageDesc>& m_images;

	}; // ImageDescLoader

	/************************************************************************/
	/* page desc                                                            */
	/************************************************************************/

	struct PageDesc
	{
		int min, max;

		Page* page;

		PageDesc() : page(NULL) {}
	};

	class PageDescLoader : public FileLoader
	{
	public:
		PageDescLoader(const std::string& filepath, std::map<std::string, uint32_t>& export_names,
			std::vector<PageDesc>& pages);

	protected:
		virtual void OnLoad(ImportStream& is);

	private:
		std::map<std::string, uint32_t>& m_export_names;

		std::vector<PageDesc>& m_pages;

	}; // PageDescLoader

protected:
	const std::string m_filepath;

	std::vector<ImageDesc> m_images;

	std::map<std::string, uint32_t> m_export_names;

	std::vector<PageDesc> m_pages;

}; // Package

}

#endif // _SIMP_PACKAGE_H_