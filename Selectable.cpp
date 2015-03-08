#include "Selectable.h"

/*
====================

adjustRepresentationIfSelected()

	Modulate the object's texture with the _modColor (defined in Selectable.h) if selected; otherwise undo modulation.

====================
*/
void Selectable::adjustRepresentationIfSelected() {
	if ( selected ) {
		_representation->modulate( _modColor );
	} else {
		_representation->resetModulation();
	}
}
