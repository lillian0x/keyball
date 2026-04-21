/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    ALT_T(KC_TAB), KC_QUOT  , KC_COMM  , KC_DOT   , KC_P     , KC_Y     ,                                  KC_F     , KC_G     , KC_C     , KC_R     , KC_L     , KC_EQL   ,
    CTL_T(KC_ESC)   , KC_A     , KC_O     , KC_E     , KC_U     , KC_I     ,                                  KC_D     , KC_H     , KC_T     , KC_N     , KC_S     , KC_MINS  ,
    KC_LWIN  , KC_SCLN  , KC_Q     , KC_J     , KC_K     , KC_X     ,                                  KC_B     , KC_M     , KC_W     , KC_V     , KC_Z     , KC_BSLS  ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),            LT(3,KC_BSPC),LT(2,KC_ENT), RSFT_T(KC_SPC),     KC_RWIN  , KC_PSCR
  ),

  [1] = LAYOUT_universal(
    _______ ,  KC_F1   , KC_F2    , KC_F3   , KC_F4    , KC_F5    ,                                   KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    _______ ,  KC_BTN3, KC_BTN2 , KC_UP   , KC_BTN1   , KC_DEL   ,                                   KC_PGUP  , KC_BTN1  , KC_UP    , KC_BTN2  , KC_BTN3  , KC_F12   ,
    _______ ,  KC_BTN4 , KC_LEFT  , KC_DOWN , KC_RGHT  , KC_BTN5  ,                                   KC_BTN5  , KC_LEFT  , KC_DOWN  , KC_RGHT  , KC_BTN4  , _______  ,
                  _______  , _______ , _______  ,         _______  , _______  ,             _______  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  ,S(KC_QUOT), KC_7     , KC_8    , KC_9     , S(KC_8)  ,                                   S(KC_9)  , S(KC_1)  , S(KC_6)  , KC_LBRC  , S(KC_4)  , _______  ,
    _______  ,S(KC_SCLN), KC_4     , KC_5    , KC_6     , KC_RBRC  ,                                   KC_NUHS  , KC_MINS  , S(KC_EQL), S(KC_3)  , KC_QUOT  , S(KC_2)  ,
    _______  ,S(KC_MINS), KC_1     , KC_2    , KC_3     ,S(KC_RBRC),                                  S(KC_NUHS),S(KC_INT1), KC_EQL   ,S(KC_LBRC),S(KC_SLSH),S(KC_INT3),
                  KC_0     , _______  , _______  ,         _______  , _______  ,             _______   , _______  , _______       , _______  , _______
  ),

  [3] = LAYOUT_universal(
    _______  ,_______   , LGUI(KC_7)  , LGUI(KC_8)  , LGUI(KC_9)  , _______  ,                                  SCRL_DVD  , SCRL_DVI  , _______  , _______ , SSNP_FRE ,KBC_RST  ,
    _______  , _______  , LGUI(KC_4)  , LGUI(KC_5)  , LGUI(KC_6)  , _______ ,                                  AML_D50  , AML_I50  , _______  , _______ , SSNP_VRT  , AML_TO  ,
    _______ , _______  , LGUI(KC_1)  , LGUI(KC_2)  , LGUI(KC_3)  , _______ ,                                  CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , SSNP_HOR  , KBC_SAVE ,
                  LGUI(KC_0)  , _______  , _______  ,        _______  , _______  ,             _______  , _______  , _______       , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
