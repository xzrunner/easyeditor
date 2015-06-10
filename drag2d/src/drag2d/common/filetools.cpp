#include "FileTools.h"

#include <wx/log.h>
#include <wx/dir.h>
#include <wx/filename.h>

namespace d2d
{

bool rm_dir(const std::string& _dir_path) 
{
	wxString dir_path(_dir_path);

   // *************************************************************
   // COMMENT THIS BLOCK OUT IF YOU WANT TO BE ABLE TO DELETE ROOT
   // DIRECTORIES : IT IS RECOMMENDED YOU LEAVE IT AS IS
   // *************************************************************
   // BEGIN SAFETY CHECK
   // *************************************************************
   ///*
   // if it's a possible root directory
   if (dir_path.length() <= 3) {
      wxLogError("You asked RemDirRF() to delete a possible root directory.\
  This is disabled by default.  If you really need to delete \"" +
         dir_path + "\" please alter the RemDirRF() definition.");

      return false;
   }
   //*/
   // *************************************************************
   // END SAFETY CHECK
   // *************************************************************

    // first make sure that the dir exists
    if(!wxDir::Exists(dir_path)) {
            wxLogError(dir_path + " does not exist.  Could not remove directory.");
            return false;
    }
       
   // append a slash if we don't have one
   if (dir_path[dir_path.length()-1] != wxFILE_SEP_PATH) {
      dir_path += wxFILE_SEP_PATH;       
    }

   // define our directory object.  When we begin traversing it, the
   // os will not let go until the object goes out of scope.
    wxDir* dir = new wxDir(dir_path);

   // check for allocation failure
   if (dir == NULL) {
      wxLogError("Could not allocate new memory on the heap!");
      return false;
   }

   // our file name temp var
    wxString filename;
   // get the first filename
    bool cont = dir->GetFirst(&filename);

   // if there are files to process
    if (cont){
        do {
         // if the next filename is actually a directory
            if (wxDirExists(dir_path + filename)) {
            // delete this directory
                rm_dir((dir_path + filename).ToStdString());
            }
            else {
            // otherwise attempt to delete this file
                if(!wxRemoveFile(dir_path + filename)) {
               // error if we couldn't
               wxLogError("Could not remove file \"" + dir_path + filename + "\"");
            }
            }
        }
      // get the next file name
        while (dir->GetNext(&filename));
    }   

   // Remove our directory object, so the OS will let go of it and
   // allow us to delete it
   delete dir;

   // now actually try to delete it
   if (!wxFileName::Rmdir(dir_path)) {
      // error if we couldn't
      wxLogError("Could not remove directory " + dir_path);
      // return accordingly
      return false;
   }
   // otherwise
   else {
      // return that we were successfull.
      return true; 
   }
}

bool mk_dir(const std::string& dir, bool rm)
{
	bool ret = true;
	if (wxDir::Exists(dir)) {
		if (rm) {
			rm_dir(dir);
			ret = wxDir::Make(dir);
		}
	} else {
		ret = wxDir::Make(dir);
	}
	return ret;
}

}