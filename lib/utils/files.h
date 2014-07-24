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
#ifndef __FILES_H__
#define __FILES_H__

#include <fstream>
#include <string>
#include <cerrno>
#include <iostream>
#include <fstream>
#include <vector>

namespace files {

	int writeFile(const std::string& iFileName, const std::string& iData);

	int isDirectory(const std::string& iPath);
	void listFilesFromDirectory(const std::string& iDir, std::vector<std::string>& ioList);

	std::string get_file_contents(const std::string& iFilename);
	std::string get_file_contents(const char *filename);

	std::string get_base_name(const std::string& iFileName);
	std::string get_dir_name(const std::string& iFileName);
	int get_real_path(const std::string& iPathName, std::string& ioRealPath);

	bool exists(const std::string& filename);
	std::string get_file_extension(const std::string& filename);

}


#endif
