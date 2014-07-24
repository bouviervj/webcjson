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
#ifndef __COMPRESS_H__
#define __COMPRESS_H__

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <sstream> 

namespace cmp {

	void zlib_compress_memory(const char* in_data, size_t in_data_size, std::ostringstream& ioStream);
}

#endif
