/**
 * @file board.c
 * @author Lingfeng Peng (uint8_t@163.com)
 * @brief
 * @version 0.1
 * @date 2022-04-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "board.h"
#include "key.h"
#include "led.h"
#include "dht11.h"

extern void sntp_start();
void board_init(void)
{
    led_init();
    key_init();
    dht11_init();
    sntp_start();
}

// static board_init_t board_init_head = NULL;
// #define MAX_INIT_NUM 8
// board_init_t board_init_node[MAX_INIT_NUM];
// static uint32_t count;

// void board_init(void)
// {
//     INIT_REGISTER(key_init);
//     INIT_REGISTER(dht11_init);
//     INIT_REGISTER(led_init);
//     board_init_start();
// }

// void board_init_register(board_init_func func)
// {
//     board_init_node[count] = (board_init_t)heap_caps_malloc(sizeof(board_init_t), MALLOC_CAP_8BIT);
//     if (board_init_node[count]) {
//         board_init_node[count]->cb = func;

//         board_init_node[count]->next = board_init_head;
//         board_init_head = board_init_node[count];
//         printf("board_init_node[%d]:%#x\n", count, (uint32_t)board_init_node[count]);
//         printf("board_init_node[%d]->cb:%#x\n", count, (uint32_t)board_init_node[count]->cb);

//         count++;
//     }
// }

// void board_init_start(void)
// {
//     for (board_init_t node = board_init_head; node; node = node->next) {
//         if (node->cb) {
//             printf("node:%#x\n", (uint32_t)node);
//             printf("node->cb:%#x\n",(uint32_t)node->cb);
//             node->cb();
//         }
//     }
// }
