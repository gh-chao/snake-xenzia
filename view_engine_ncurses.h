#include "game.h"

#ifndef SNAKE_XENZIA_VIEW_ENGINE_NCURSES_H
#define SNAKE_XENZIA_VIEW_ENGINE_NCURSES_H
#endif //SNAKE_XENZIA_VIEW_ENGINE_NCURSES_H

void view_engine_ncursess_init(view_engine_s *view_engine);
void view_engine_ncursess_construct(view_engine_s *view_engine);
void view_engine_ncursess_destruct(view_engine_s *view_engine);
void view_engine_ncursess_render_welcome(game_s *game);
void view_engine_ncursess_render_body(game_s *game);
void view_engine_ncursess_render_game_over(game_s *game);
int view_engine_ncursess_read_input_key_code();