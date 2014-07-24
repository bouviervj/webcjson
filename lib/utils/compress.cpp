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
#include "compress.h"

#include <zlib.h>

namespace cmp {

	void zlib_compress_memory(const char* in_data, size_t in_data_size, std::ostringstream& ioStream) {

		 //std::vector<uint8_t> buffer;

		 const size_t BUFSIZE = 128 * 1024;
		 unsigned char temp_buffer[BUFSIZE];

		 z_stream strm;
		 strm.zalloc = 0;
		 strm.zfree = 0;
		 strm.next_in = (unsigned char*) (in_data);
		 strm.avail_in = in_data_size;
		 strm.next_out = temp_buffer;
		 strm.avail_out = BUFSIZE;

		 deflateInit(&strm, Z_BEST_COMPRESSION);

		 while (strm.avail_in != 0)
		 {
		  int res = deflate(&strm, Z_NO_FLUSH);
		  assert(res == Z_OK);
		  if (strm.avail_out == 0)
		  {
		   ioStream.write((char*) temp_buffer,BUFSIZE);
		   //buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
		   strm.next_out = temp_buffer;
		   strm.avail_out = BUFSIZE;
		  }
		 }

		 int deflate_res = Z_OK;
		 while (deflate_res == Z_OK)
		 {
		  if (strm.avail_out == 0)
		  {
		   //buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
		   ioStream.write((char*) temp_buffer,BUFSIZE);
		   strm.next_out = temp_buffer;
		   strm.avail_out = BUFSIZE;
		  }
		  deflate_res = deflate(&strm, Z_FINISH);
		 }

		 assert(deflate_res == Z_STREAM_END);
		 //buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
		 ioStream.write((char*) temp_buffer,BUFSIZE-strm.avail_out);
		 deflateEnd(&strm);

		// out_data.swap(buffer);
	}

}
