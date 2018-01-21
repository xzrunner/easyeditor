#pragma once

#include <node3/IModel.h>

namespace enode3d
{

class Model : public n3::IModel
{
public:

	void SetFilepath(const std::string& filepath) { m_filepath = filepath; }
	const std::string& GetFilepath() const { return m_filepath; }

private:
	std::string m_filepath;

}; // Model

}