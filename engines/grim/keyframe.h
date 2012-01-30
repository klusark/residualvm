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

#ifndef GRIM_KEYFRAME_H
#define GRIM_KEYFRAME_H

#include "math/vector3d.h"

#include "engines/grim/object.h"
#include "engines/grim/tmpresource.h"

namespace Common {
class SeekableReadStream;
}

namespace Grim {

class ModelNode;
class TextSplitter;

class KeyframeAnimData : public SharedData<KeyframeAnimData>
{
public:
	KeyframeAnimData();
	~KeyframeAnimData();

	void loadBinary(Common::SeekableReadStream *data);
	void loadText(TextSplitter &ts);
	bool load(Common::SeekableReadStream *data);
	float getLength() const { return _numFrames / _fps; }

	bool animate(ModelNode *nodes, int num, float time, float fade, bool tagged) const;
	int getMarker(float startTime, float stopTime) const;

	unsigned int _flags;
	/**
	 * A bitfield ID which specifies which joints of the skeleton hierarchy this
	 * KeyFrameAnim can animate on. This is ANDed against the _type of the ModelNode
	 * to test whether this KeyFrameAnim can animate that ModelNode, or if it is to
	 * be ignored.
	 */
	unsigned int _type;
	int _numFrames, _numJoints;
	float _fps;
	int _numMarkers;

	struct Marker {
		float frame;
		int val;
	};
	Marker *_markers;

	struct KeyframeEntry {
		void loadBinary(const char *data);

		float _frame;
		int _flags;
		Math::Vector3d _pos, _dpos;
		Math::Angle _pitch, _yaw, _roll, _dpitch, _dyaw, _droll;
	};

	struct KeyframeNode {
		void loadBinary(Common::SeekableReadStream *data, char *meshName);
		void loadText(TextSplitter &ts);
		~KeyframeNode();

		bool animate(ModelNode &node, float frame, float fade, bool useDelta) const;

		char _meshName[32];
		int _numEntries;
		KeyframeEntry *_entries;
	};

	KeyframeNode **_nodes;
};

class KeyframeAnim : public Object, public LoadableResource<KeyframeAnim, KeyframeAnimData> {
public:
	KeyframeAnim(const Common::String &filename);

	bool animate(ModelNode *nodes, int num, float time, float fade, bool tagged) const { requireLoaded(); return _data->animate(nodes, num, time, fade, tagged); }
	int getMarker(float startTime, float stopTime) const { requireLoaded(); return _data->getMarker(startTime, stopTime); }

	float getLength() const { requireLoaded(); return _data->getLength(); }


};

} // end of namespace Grim

#endif
