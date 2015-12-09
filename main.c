#include <ncurses.h>
#include "ui_ncurses.h"

int main() {
    // 初始化ui
    ui_s ui;
    ui_ncurses_init(&ui);
    ui_ncurses_construct(&ui);

    // 游戏检测
    if (ui.scene_x < 30 || ui.scene_y < 30) {
        ui_ncurses_destruct(&ui);
        printf("游戏场景不能小于30*30\n");
    } else {
        // 创建游戏
        game_s snake_xenzia;
        // 初始化游戏
        game_construct(&snake_xenzia, &ui);
        // 游戏开始
        game_start(&snake_xenzia);
        // 游戏结束
        game_destruct(&snake_xenzia);
    }

    // 视图销毁
    ui_ncurses_destruct(&ui);
    return 0;
}



