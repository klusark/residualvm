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

#ifndef GRIM_COLORMAP_H
#define GRIM_COLORMAP_H

#include "engines/grim/object.h"
#include "engines/grim/tmpresource.h"

namespace Common {
class SeekableReadStream;
}

namespace Grim {

class CMapData : public SharedData<CMapData> {
public:
	// Load a colormap from the given data.
	CMapData();

	bool load(Common::SeekableReadStream *data);

	// The color data, in RGB format
	char _colors[256 * 3];

	bool operator==(const CMap &c) const;
};

class CMap : public Object, public LoadableResource<CMap, CMapData> {
public:
	CMap(const Common::String &name) : LoadableResource<CMap, CMapData>(name) {}
	//bool operator==(const CMap &c) const { return true;}
	const char *getColors() const { requireLoaded(); return _data->_colors; };
};

} // end of namespace Grim

#endif
