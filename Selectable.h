#pragma once
#include "Drawable.h"
#include "Texture.h"

class Selectable : public virtual Drawable {
public:
	bool				selected;
						Selectable( SDLWrapper::Texture textureTemplate ) : selected(false), _texture( textureTemplate ) , _modColor ( Color( 128, 255, 128 ) ) {}
	SDLWrapper::Texture	texture() { return _texture; }
	virtual bool		select() { selected = true; return true; } // override to specify other behavior
	virtual void		deselect() { selected = false; }

protected:
	SDLWrapper::Texture	_texture;
	Color				_modColor;
	virtual void		adjustRepresentationIfSelected();
};
