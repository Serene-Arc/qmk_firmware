#include QMK_KEYBOARD_H


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _MATH 0

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  SQUARE,
  INVERSE,
  IND1,
  IND2,
  DOT,
  PART,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_MATH] =  LAYOUT( \
  QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
  SQUARE,  INVERSE, IND1, IND2, DOT,  PART,    _______, _______, _______, _______, _______, _______ \
)


};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
	  case SQUARE:
		  if (record->event.pressed){
			  SEND_STRING("^2");
		  }
		  break;
	  case INVERSE:
		  if (record->event.pressed){
			  SEND_STRING("^{-1}");
		  }
		  break;
	  case IND1:
		  if (record->event.pressed){
			  SEND_STRING("_1");
		  }
		  break;
	  case IND2:
		  if (record->event.pressed){
			  SEND_STRING("_2");
		  }
		  break;
	  case DOT:
		  if (record->event.pressed){
			  SEND_STRING("\\dot{");
		  }
		  break;
	  case PART:
		  if (record->event.pressed){
			  SEND_STRING("\\partial ");
		  }
		  break;
  }
  return true;
}
