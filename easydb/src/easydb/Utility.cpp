#include "Utility.h"

#include <wx/utils.h>

namespace edb
{

void Utility::OpenPath(const std::string& filepath)
{
	std::string command = "explorer /select, " + filepath;
	wxExecute(command, wxEXEC_ASYNC, NULL);
}

}