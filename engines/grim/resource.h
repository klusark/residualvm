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

#ifndef GRIM_RESOURCE_H
#define GRIM_RESOURCE_H

#include "common/archive.h"
#include "common/file.h"

#include "engines/grim/object.h"
#include "engines/grim/lua/lua.h"

namespace Grim {

class Bitmap;
class CMap;
class Costume;
class Font;
class KeyframeAnim;
class Material;
class Model;
class EMIModel;
class LipSync;
class TrackedObject;
class SaveGame;
class Skeleton;
class Lab;

typedef ObjectPtr<Material> MaterialPtr;
typedef ObjectPtr<Bitmap> BitmapPtr;
typedef ObjectPtr<Model> ModelPtr;
typedef ObjectPtr<CMap> CMapPtr;
typedef ObjectPtr<KeyframeAnim> KeyframeAnimPtr;
typedef ObjectPtr<Font> FontPtr;
typedef ObjectPtr<LipSync> LipSyncPtr;

class ResourceLoader {
public:
	ResourceLoader();

	Costume *loadCostume(const Common::String &fname, Costume *prevCost);
	//Material *loadMaterial(const Common::String &fname, CMap *c);
	Model *loadModel(const Common::String &fname, CMap *c, Model *parent = NULL);
	EMIModel *loadModelEMI(const Common::String &fname, EMIModel *parent = NULL);
	Skeleton *loadSkeleton(const Common::String &fname);
	Common::SeekableReadStream *openNewStreamFile(Common::String fname);
	bool getFileExists(const Common::String &filename);  //TODO: make it const again at next scummvm sync

	ModelPtr getModel(const Common::String &fname, CMap *c);

private:
	Common::SeekableReadStream *loadFile(Common::String &filename);  //TODO: make it const again at next scummvm sync
	void loadPatches();

	Common::SearchSet _files;
	Common::List<Common::String> _patches;


};

extern ResourceLoader *g_resourceloader;

} // end of namespace Grim

#endif
