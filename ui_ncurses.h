#include "game.h"

#ifndef SNAKE_XENZIA_UI_NCURSES_H
#define SNAKE_XENZIA_UI_NCURSES_H


void ui_ncurses_init(ui_s *s);
void ui_ncurses_construct(ui_s *this);
void ui_ncurses_destruct(ui_s *this);
void ui_ncurses_render_welcome(ui_s *this, game_s *game);
void ui_ncurses_render_body(ui_s *this, game_s *game);
void ui_ncurses_render_game_over(ui_s *this, game_s *game);
int ui_ncurses_read_control(ui_s *this);

#endif //SNAKE_XENZIA_UI_NCURSES_H