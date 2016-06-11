/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2016 The GemRB Project
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
 *
 */

#ifndef TIMER_H
#define TIMER_H

#include "Callback.h"

#include "globals.h"

namespace GemRB {

class GEM_EXPORT Timer {
public:
	typedef unsigned long TimeInterval;

private:
	TimeInterval interval;
	TimeInterval fireDate;
	bool valid;
	EventHandler action;

private:
	void NextFireDate() {
		fireDate = GetTickCount() + interval;
	}

public:
	Timer(TimeInterval i, const EventHandler& handler)
	: action(handler)
	{
		valid = true;
		interval = i;
		NextFireDate();
	}

	void Update() {
		TimeInterval now = GetTickCount();
		if (fireDate <= now) {
			action();
			NextFireDate();
		}
	}

	void Invalidate() {
		valid = false;
	}

	bool IsRunning() const {
		return valid;
	}
};

}

#endif
