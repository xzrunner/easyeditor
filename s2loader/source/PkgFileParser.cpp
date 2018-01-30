#include "s2loader/PkgFileParser.h"

#include <fs_file.h>
#include <simp/NodeFactory.h>
#include <timp/PkgMgr.h>
#include <timp/Package.h>

namespace s2loader
{

uint32_t PkgFileParser::GetEpeIdxOffset(fs_file* file)
{
	fs_seek_from_head(file, sizeof(uint16_t) * 2);

	uint32_t offset = 0;
	fs_read(file, &offset, sizeof(uint32_t));
	return offset;
}

uint32_t PkgFileParser::GetEptIdxOffset(fs_file* file)
{
	int n_epe = 0;
	fs_seek_from_head(file, 0);
	fs_read(file, &n_epe, sizeof(uint16_t));
	fs_seek_from_head(file, sizeof(uint16_t) * 2 + sizeof(uint32_t) * n_epe);

	uint32_t offset = 0;
	fs_read(file, &offset, sizeof(uint32_t));
	return offset;
}

void PkgFileParser::SetEPPath(fs_file* file, const CU_STR& filepath, int pkg_id)
{
	const simp::Package* s_pkg = simp::NodeFactory::Instance()->QueryPkg(pkg_id);
	if (!s_pkg) {
		return;
	}
	const timp::Package* t_pkg = timp::PkgMgr::Instance()->Query(pkg_id);
	if (!t_pkg) {
		return;
	}

	int n_epe = 0, n_ept = 0;
	fs_seek_from_head(file, 0);
	fs_read(file, &n_epe, sizeof(uint16_t));
	fs_read(file, &n_ept, sizeof(uint16_t));

	uint32_t offset = 0;
	fs_read(file, &offset, sizeof(offset));
	for (int i = 1; i < n_epe; ++i) 
	{
		fs_read(file, &offset, sizeof(offset));
		const_cast<simp::Package*>(s_pkg)->SetPagePath(i - 1, bimp::FilePath(filepath, offset));
	}
	fs_read(file, &offset, sizeof(offset));
	for (int i = 1; i < n_ept; ++i) {
		fs_read(file, &offset, sizeof(offset));
		const_cast<timp::Package*>(t_pkg)->SetTexPath(i - 1, 0, bimp::FilePath(filepath, offset));
	}
}

}