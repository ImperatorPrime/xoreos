/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010-2011 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file graphics/aurora/text.cpp
 *  A text object.
 */

#include "events/requests.h"

#include "graphics/graphics.h"

#include "graphics/aurora/text.h"
#include "graphics/aurora/font.h"

namespace Graphics {

namespace Aurora {

Text::Text(const FontHandle &font, const Common::UString &str) :
	_font(font), _x(0.0), _y(0.0), _list(0) {

	set(str);

	_distance = -5.0;
}

Text::~Text() {
}

void Text::set(const Common::UString &str) {
	GfxMan.lockFrame();

	bool visible = Renderable::isInQueue();

	Renderable::removeFromQueue();

	set_internal(str);

	if (visible)
		Renderable::addToQueue();

	RequestMan.dispatchAndForget(RequestMan.buildLists(this));

	GfxMan.unlockFrame();
}

void Text::set_internal(const Common::UString &str) {
	_str = str;

	const Font &font = _font.getFont();

	_height = font.getHeight(_str);
	_width  = font.getWidth (_str);
}

void Text::setPosition(float x, float y) {
	GfxMan.lockFrame();

	bool visible = Renderable::isInQueue();

	Renderable::removeFromQueue();

	_x = roundf(x * 100.0) / 100.0;
	_y = roundf(y * 100.0) / 100.0;

	if (visible)
		Renderable::addToQueue();

	GfxMan.unlockFrame();
}

void Text::show() {
	Renderable::addToQueue();
}

void Text::hide() {
	Renderable::removeFromQueue();
}

float Text::getWidth() const {
	return _width;
}

float Text::getHeight() const {
	return _height;
}

void Text::newFrame() {
}

void Text::render() {
	glTranslatef(_x * 100.0, _y * 100.0, 0.0);

	if (_list > 0)
		glCallList(_list);
}

void Text::rebuild() {
	enforceMainThread();

	const Font &font = _font.getFont();

	_list = glGenLists(1);

	glNewList(_list, GL_COMPILE);
	font.draw(_str);
	glEndList();
}

void Text::destroy() {
	enforceMainThread();

	if (_list == 0)
		return;

	glDeleteLists(_list, 0);

	_list = 0;
}

} // End of namespace Aurora

} // End of namespace Graphics
