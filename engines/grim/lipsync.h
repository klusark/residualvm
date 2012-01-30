/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 */

#ifndef GRIM_LIPSYNC_H
#define GRIM_LIPSYNC_H

#include "engines/grim/object.h"
#include "engines/grim/tmpresource.h"

namespace Common {
class SeekableReadStream;
}

namespace Grim {

class LipSyncData : public SharedData<LipSyncData> {
public:
	LipSyncData();
	~LipSyncData();

	bool load(Common::SeekableReadStream *data);

	struct LipEntry {
		uint16 frame;
		uint16 anim;
	};

	int getAnim(int pos) const;
	bool isValid() const { return _numEntries > 0; }


private:
	LipEntry *_entries;
	int _numEntries;

	struct PhonemeAnim {
		uint16 phoneme;
		uint16 anim;
	};

	static const PhonemeAnim _animTable[];
	static const int _animTableSize;
};

class LipSync : public Object, public LoadableResource<LipSync, LipSyncData> {
public:
	LipSync(const Common::String &name) : LoadableResource<LipSync, LipSyncData>(name) {};

	int getAnim(int pos) const { requireLoaded(); return _data->getAnim(pos); }
	bool isValid() const { requireLoaded(); return _data->isValid(); }

};

} // end of namespace Grim

#endif

