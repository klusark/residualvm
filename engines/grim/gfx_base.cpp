/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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

#include "engines/grim/gfx_base.h"
#include "engines/grim/savegame.h"

#include "engines/grim/model.h"

namespace Grim {

GfxBase::GfxBase() :
		_renderBitmaps(true), _renderZBitmaps(true), _shadowModeActive(false),
		_currentPos(0, 0, 0), _currentQuat(0, 0, 0, 1), _dimLevel(0.0f),
		_screenWidth(0), _screenHeight(0), _screenSize(0), _isFullscreen(false),
		_scaleW(1.0f), _scaleH(1.0f), _currentShadowArray(NULL),
		_shadowColorR(255), _shadowColorG(255), _shadowColorB(255) {

}

void GfxBase::setShadowMode() {
	_shadowModeActive = true;
}

void GfxBase::clearShadowMode() {
	_shadowModeActive = false;
}

bool GfxBase::isShadowModeActive() {
	return _shadowModeActive;
}

void GfxBase::saveState(SaveGame *state) {
	state->beginSection('DRVR');

	byte r, g, b;
	getShadowColor(&r, &g, &b);
	state->writeByte(r);
	state->writeByte(g);
	state->writeByte(b);
	state->writeBool(_renderBitmaps);
	state->writeBool(_renderZBitmaps);

	state->endSection();
}

void GfxBase::restoreState(SaveGame *state) {
	state->beginSection('DRVR');

	byte r, g, b;
	r = state->readByte();
	g = state->readByte();
	b = state->readByte();
	setShadowColor(r, g , b);
	_renderBitmaps = state->readBool();
	_renderZBitmaps = state->readBool();

	state->endSection();
}

void GfxBase::renderBitmaps(bool render) {
	_renderBitmaps = render;
}

void GfxBase::renderZBitmaps(bool render) {
	_renderZBitmaps = render;
}

void GfxBase::drawMesh(const Mesh *mesh) {
	for (int i = 0; i < mesh->_numFaces; i++)
		mesh->_faces[i].draw(mesh);
}

// Prototypes for the factory-function below.
GfxBase *CreateGfxOpenGL();
GfxBase *CreateGfxOpenGLS();
GfxBase *CreateGfxTinyGL();
GfxBase *CreateGfxDriver(RendererType preferredRendererType) {
#if defined(USE_OPENGL) && !defined(USE_GLES2)
	if (preferredRendererType == GFX_DRIVER_OPENGL) {
		return CreateGfxOpenGL();
	}
#endif

#ifdef USE_OPENGL_SHADERS
	if (preferredRendererType == GFX_DRIVER_OPENGL_SHADERS) {
		return CreateGfxOpenGLS();
	}
#endif

	if (preferredRendererType == GFX_DRIVER_TINYGL) {
		return CreateGfxTinyGL();
	}
	// Fallthrough, in case the relevant if-block above was disabled.
	return CreateGfxTinyGL();
}

void SpecialtyMaterial::select() const {
	if (_texture) {
		g_driver->selectMaterial(_texture);
	}
}

void SpecialtyMaterial::create(const char *data, int width, int height) {
	delete _texture;
	_texture = new Texture();
	_texture->_width = width;
	_texture->_height = height;
	_texture->_bpp = 4;
	_texture->_colorFormat = BM_RGBA;
	g_driver->createMaterial(_texture, data, NULL);
}

}
