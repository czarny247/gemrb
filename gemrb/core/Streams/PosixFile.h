/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2023 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef POSIX_FILE_H
#define POSIX_FILE_H

#include <cstdio>

#include "File.h"

namespace GemRB {

class GEM_EXPORT PosixFile : public File {
private:
	FILE* file = nullptr;
public:
	PosixFile() noexcept = default;
	PosixFile(const File&) = delete;
	PosixFile(PosixFile&& f) noexcept;
	~PosixFile() override;

	PosixFile& operator=(const PosixFile&) = delete;
	PosixFile& operator=(PosixFile&& f) noexcept;

	strpos_t Length() override;
	bool OpenRO(const path_t& name) override;
	bool OpenRW(const path_t& name) override;
	bool OpenNew(const path_t& name) override;
	strret_t Read(void* ptr, size_t length) override;
	strret_t Write(const void* ptr, strpos_t length) override;
	bool SeekStart(stroff_t offset) override;
	bool SeekCurrent(stroff_t offset) override;
	bool SeekEnd(stroff_t offset) override;
};

}
#endif
