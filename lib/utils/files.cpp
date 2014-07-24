/*
 * Copyright 2014 BOUVIER-VOLAILLE Julien
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "files.h"

#include <string.h> 
#include <stdlib.h>
#include <libgen.h>

#include <linux/limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>


namespace files {

	int writeFile(const std::string& iFileName, const std::string& iData){

		std::ofstream file;
		file.open (iFileName.c_str(), std::ios::trunc | std::ios::out);
		file << iData;
		file.close();

	}

	int isDirectory(const std::string& iPath) {
		struct stat s;
		if( stat(iPath.c_str(),&s) == 0 )
		{
		    if( s.st_mode & S_IFDIR )
		    {
			  return 1;
		    }
		}
		else
		{
		   return 2;
		}
		return 0;
	}

	void listFilesFromDirectory(const std::string& iDir, std::vector<std::string>& ioList){

	   if (isDirectory(iDir)) {
		   DIR* dirp = opendir(iDir.c_str());
		   if (dirp!=0) {
			   struct dirent* dp = 0;
			   while ((dp = readdir(dirp)) != NULL) {
				ioList.push_back(std::string(dp->d_name));
			   }
			   (void)closedir(dirp);
		   }
	   }

	}
	
	std::string get_file_contents(const std::string& iFilename) {
		return get_file_contents(iFilename.c_str());
	}

	std::string get_file_contents(const char *filename) {
	  std::ifstream in(filename, std::ios::in | std::ios::binary);
	  if (in)
	  {
	    std::string contents;
	    in.seekg(0, std::ios::end);
	    contents.resize(in.tellg());
	    in.seekg(0, std::ios::beg);
	    in.read(&contents[0], contents.size());
	    in.close();
	    return(contents);
	  }
	  throw(errno);
	}

	std::string get_file_extension(const std::string& filename) {
		return filename.substr(filename.find_last_of(".") + 1);
  	}

	std::string get_dir_name(const std::string& iFileName) {
	    int size = iFileName.size();
	    char* buffer = new char[size+1];
	    memcpy(buffer,iFileName.c_str(),size);
	    buffer[size] = 0; 	
	    std::string result(dirname(buffer));
	    free(buffer);	
	    return result;	
	}

	std::string get_base_name(const std::string& iFileName) {
	    int size = iFileName.size();
	    char* buffer = new char[size+1];
	    memcpy(buffer,iFileName.c_str(),size);
	    buffer[size] = 0; 	
	    std::string result(basename(buffer));
	    free(buffer);	
	    return result;	
	}
	
	int get_real_path(const std::string& iPathName, std::string& ioRealPath) {
	    char actualpath [PATH_MAX+1];
	    char *ptr;
	    ptr = realpath(iPathName.c_str(), actualpath);
	    if (ptr!=0) {
		 ioRealPath = std::string(ptr);
	      return 1;
	    }
	    return 0;
	}

	bool exists(const std::string& filename) {
	    if (FILE * file = fopen(filename.c_str(), "r")) //I'm sure, you meant for READING =)
	    {
		   fclose(file);
		   return true;
	    }
	    return false;
	}

}

