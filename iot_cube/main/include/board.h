/**
 * @file board.h
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "esp_heap_caps.h"

typedef struct _board_init_t *board_init_t;

typedef int (*board_init_func)(void);

struct _board_init_t {
    struct _board_init_t *next;
    board_init_func cb;
};

void board_init(void);
void board_init_register(board_init_func func);
void board_init_start(void);

#define INIT_REGISTER board_init_register


