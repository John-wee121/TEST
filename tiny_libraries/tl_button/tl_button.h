/**
 * @file tl_button.h
 * @author john (1973372075@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TL_BUTTON_H
#define TL_BUTTON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TL_BTN_NONE_FLAG (0x0000)
#define TL_BTN_UP_FLAG (0x0001)
#define TL_BTN_DOWN_FLAG (0x0002)
#define TL_BTN_CLICK_FLAG (0x0004)
#define TL_BTN_LONG_FLAG (0x0008)
#define TL_BTN_MULTIPLE_FLAG (0x0010)

    // 回调函数类型
    typedef uint8_t (*btn_io_cb_t)(void);
    typedef void (*btn_event_cb_t)(void);

    // IO电平
    enum tl_btn_io_level
    {
        eTL_BTN_IO_LV_LOW = 0,
        eTL_BTN_IO_LV_HIGH,
    };

    // 编码器事件回调列表
    enum tl_btn_event_cb
    {
        eTL_BTN_EVENT_CB_KEY_UP = 0,
        eTL_BTN_EVENT_CB_KEY_DOWN,
        eTL_BTN_EVENT_CB_KEY_CLICK,
        eTL_BTN_EVENT_CB_KEY_LONG,
        eTL_BTN_EVENT_CB_KEY_MULTIPLE,

        eTL_BTN_EVENT_CB_MAX,
    };

    // 编码器结构体对象
    struct tl_button
    {
        uint8_t name[8]; // 名字
        uint8_t id;      // ID

        uint32_t trigger_time;  // 触发计时时间线
        uint32_t debounce_time; // 消抖时间
        uint32_t long_time;     // 长按触发时间
        uint32_t reset_time;    // 重置计时

        uint16_t callback_flag; // 回调标记
        uint8_t multiple;       // 多击计数
        uint8_t trigger_level;  // 触发电平

        uint8_t last_state; // 上一次按键状态
        uint8_t state;      // 按键状态

        struct tl_button *next; // 链接指针

        btn_io_cb_t io_cb;                             // IO读取回调函数
        btn_event_cb_t event_cb[eTL_BTN_EVENT_CB_MAX]; // 事件回调函数
    };
    typedef struct tl_button tl_button_t;

    // 应用编程接口
    uint8_t tl_btn_create(const uint8_t *name,
                          tl_button_t *_dev,
                          const uint8_t _trigger_level,
                          const uint32_t _long_time);
    uint8_t tl_btn_io_callback_attach(tl_button_t *_dev, const btn_io_cb_t _cb);
    uint8_t tl_btn_event_callback_attach(tl_button_t *_dev, const uint8_t _event, const btn_event_cb_t _cb);
    void tl_btn_io_scan(tl_button_t *_dev);
    void tl_btn_event_resolution(tl_button_t *_dev);
    void tl_btn_callback_ops(tl_button_t *_dev);
    uint8_t tl_btn_multiple_read(const tl_button_t *_dev);
    void tl_btn_callback_flag_set(tl_button_t *_dev, const uint16_t _callback_flag);
    uint16_t tl_btn_callback_flag_get(tl_button_t *_dev, const uint16_t _callback_flag);
    void tl_btn_callback_flag_clear(tl_button_t *_dev, const uint16_t _callback_flag);
    void tl_btn_process(void);

    // 弱定义回调服务函数
    __WEAK uint8_t tl_btn_io_callback_handler(void);
    __WEAK void tl_btn_up_callback_handler(void);
    __WEAK void tl_btn_down_callback_handler(void);
    __WEAK void tl_btn_click_callback_handler(void);
    __WEAK void tl_btn_long_callback_handler(void);
    __WEAK void tl_btn_multiple_callback_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* TL_BUTTON_H */
