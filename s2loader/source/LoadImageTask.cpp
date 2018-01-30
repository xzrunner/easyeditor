#include "s2loader/LoadImageTask.h"

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <gimg_pvr.h>
#include <gimg_etc2.h>
#include <bimp/FileLoader.h>
#include <bimp/ImportStream.h>
#include <timp/TextureFormat.h>
#include <timp/TextureLoader.h>
#include <unirender/RenderContext.h>
#include <fs_file.h>
#include <gum/RenderContext.h>
#include <gum/Image.h>

#include <stdlib.h>
#include <assert.h>

namespace s2loader
{

/************************************************************************/
/* class LoadImageTask                                                  */
/************************************************************************/

LoadImageTask::LoadImageTask(const std::shared_ptr<gum::Image>& img)
	: m_img(img)
	, m_data(nullptr)
	, m_size(0)
{
	if (m_img) {
		m_img->SetLoadFinished(false);
	}	
}

LoadImageTask::~LoadImageTask()
{
	free(m_data);
}

class LoadImageTask::FileLoader : public bimp::FileLoader
{
public:
	FileLoader(const CU_STR& filepath, bool use_cache, LoadImageTask* task)
		: bimp::FileLoader(filepath, use_cache)
		, m_task(task)
	{}
	FileLoader(fs_file* file, uint32_t offset, bool use_cache, LoadImageTask* task)
		: bimp::FileLoader(file, offset, use_cache)
		, m_task(task)
	{}

protected:
	virtual void OnLoad(bimp::ImportStream& is) override
	{
		m_task->OnLoad(is);
	}

private:
	LoadImageTask* m_task;

}; // LoadImageTask::FileLoader

void LoadImageTask::Run()
{
	if (!m_img) {
		LoadImageTaskMgr::Instance()->AddResult(this);
		return;
	}
	const bimp::FilePath& path = m_img->GetResPath();
	if (path.IsSingleFile()) {
		LoadImageTask::FileLoader loader(path.GetFilepath().c_str(), false, this);
		loader.Load();
	} else {
		fs_file* file = fs_open(path.GetFilepath().c_str(), "rb");
		LoadImageTask::FileLoader loader(file, path.GetOffset(), false, this);
		loader.Load();
		fs_close(file);
	}
	LoadImageTaskMgr::Instance()->AddResult(this);
}

void LoadImageTask::Flush()
{
	if (!m_data || m_size == 0) {
		return;
	}

	timp::TextureLoader loader(static_cast<const char*>(m_data), m_size);
	loader.Load();

	const void* pixels = loader.GetData();
	int width = loader.GetWidth(),
		height = loader.GetHeight();
	int format = loader.GetFormat();
	switch (format)
	{
	case timp::TEXTURE_RGBA4: case timp::TEXTURE_RGBA8:
		gum::RenderContext::Instance()->GetImpl()->UpdateTexture(m_img->GetTexID(), pixels, width, height);
		break;
	case timp::TEXTURE_PVR2:
#if defined( __APPLE__ ) && !defined(__MACOSX)
		gum::RenderContext::Instance()->GetImpl()->UpdateTexture(m_img->GetTexID(), pixels, width, height);
#endif
		break;
	case timp::TEXTURE_PVR4:
		{
#if defined( __APPLE__ ) && !defined(__MACOSX)
			gum::RenderContext::Instance()->GetImpl()->UpdateTexture(m_img->GetTexID(), pixels, width, height);
#else
			uint8_t* rgba8 = gimg_pvr_decode_rgba8(static_cast<const uint8_t*>(pixels), width, height);
			uint8_t* rgba4 = gimg_rgba8_to_rgba4_dither(rgba8, width, height);
			gimg_revert_y((uint8_t*)rgba4, width, height, GPF_RGBA4);
			gum::RenderContext::Instance()->GetImpl()->UpdateTexture(m_img->GetTexID(), rgba4, width, height);
			free(rgba4);
			free(rgba8);
#endif
		}
		break;
	case timp::TEXTURE_ETC1:
		break;
	case timp::TEXTURE_ETC2:
		{
			ur::RenderContext* rc = gum::RenderContext::Instance()->GetImpl();
			if (rc->IsSupportETC2()) {
				rc->UpdateTexture(m_img->GetTexID(), pixels, width, height);
			} else {
				uint8_t* rgba8 = gimg_etc2_decode_rgba8(static_cast<const uint8_t*>(pixels), width, height, ETC2PACKAGE_RGBA_NO_MIPMAPS);
				uint8_t* rgba4 = gimg_rgba8_to_rgba4_dither(rgba8, width, height);
				rc->UpdateTexture(m_img->GetTexID(), rgba4, width, height);
				free(rgba4);
				free(rgba8);
			}
		}
		break;
	}
	m_img->SetLoadFinished(true);
}

void LoadImageTask::OnLoad(bimp::ImportStream& is)
{
	if (m_data) {
		free(m_data); m_data = nullptr;
	}
	m_size = is.Size();
	m_data = malloc(m_size);
	if (m_data) {
		memcpy(m_data, is.Stream(), m_size);
	} else {
		m_size = 0;
	}
}

void LoadImageTask::Initialize(const std::shared_ptr<gum::Image>& img)
{
	assert(!m_img && !m_data);
	m_img = img;
	if (m_img) {
		m_img->SetLoadFinished(false);
	}
}

void LoadImageTask::Terminate()
{
	if (m_img) {
		m_img = nullptr;
	}

	free(m_data); m_data = nullptr;
	m_size = 0;
}

/************************************************************************/
/* class LoadImageTaskMgr                                               */
/************************************************************************/

CU_SINGLETON_DEFINITION(LoadImageTaskMgr)

LoadImageTaskMgr::LoadImageTaskMgr()
	: m_count(0)
{	
}

LoadImageTask* LoadImageTaskMgr::Fetch(const std::shared_ptr<gum::Image>& img)
{
	++m_count;
	mt::Task* t = m_freelist.Front();
	LoadImageTask* tt = static_cast<LoadImageTask*>(t);
	if (!t) {
		tt = new LoadImageTask(img);
	} else {
		m_freelist.Pop();
		tt->Initialize(img);
	}
	return tt;
}

void LoadImageTaskMgr::AddResult(LoadImageTask* task)
{
	m_result.Push(task);
}

void LoadImageTaskMgr::Flush()
{
	while (mt::Task* t = m_result.TryPop())
	{
		LoadImageTask* tt = static_cast<LoadImageTask*>(t);
		tt->Flush();
		tt->Terminate();
		m_freelist.Push(t);
		--m_count;
	}
}

}