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

#ifndef GRIM_MATERIAL_H
#define GRIM_MATERIAL_H

#include "engines/grim/object.h"
#include "engines/grim/tmpresource.h"

namespace Grim {

class CMap;

class Texture {
public:
	int _width;
	int _height;
	int _colorFormat;
	int _bpp;
	bool _hasAlpha;
	void *_texture;
	char *_data;
};

class MaterialData : public SharedData<MaterialData>  {
public:
	MaterialData();
	~MaterialData();

	//Common::String _fname;
	ObjectPtr<CMap> _cmap;
	int _numImages;
	Texture *_textures;

	bool load(Common::SeekableReadStream *data);
private:
	bool initGrim(Common::SeekableReadStream *data, CMap *cmap);
	bool initEMI(Common::SeekableReadStream *data);
};

class Material : public Object, public LoadableResource<Material, MaterialData> {
public:
	// Load a texture from the given data.
	//Material(const Common::String &filename, Common::SeekableReadStream *data, CMap *cmap);
	Material(const Common::String &filename);

	void reload(CMap *cmap);
	// Load this texture into the GL context
	void select() const;

	// Set which image in an animated texture to use
	void setActiveTexture(int n);

	int getNumTextures() const;
	int getActiveTexture() const;

	//const Common::String &getFilename() const;
	MaterialData *getData() const;

	~Material();

	void postLoad() {}

private:
	//MaterialData *_data;
	int _currImage;
};

} // end of namespace Grim

#endif
