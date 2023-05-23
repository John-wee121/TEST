/**
 * @file tl_button.c
 * @author john (1973372075@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-04-12
 *
 * @copyright Copyright (c) 2023
 *
 * 按键：按下，松开，短按，长按，连击
 *
 */

#include "tl_button.h"

// 管理链表头
tl_button_t btn_head;

/**
 * @brief 创建对象
 *
 * @param name 名字
 * @param _dev 设备对象
 * @param _trigger_level 触发电平
 * @param _long_time 长按时间
 * @return uint8_t 0：成功，1：设备对象为空
 */
uint8_t tl_btn_create(const uint8_t *name,
                      tl_button_t *_dev,
                      const uint8_t _trigger_level,
                      const uint32_t _long_time)
{
    // 判断传入的对象是否合理
    // TL_ASSERT(_dev);
    if (_dev == NULL)
    {
        return 1;
    }

    // 重置head的next指针指向
    // if (btn_head.next != NULL)
    // {
    //     btn_head.next = NULL;
    // }

    // 寻找链表尾
    tl_button_t *p1 = &btn_head;
    for (; p1->next != NULL; p1 = p1->next)
    {
    }

    // 设置对象属性并添加到链表尾
    strncpy((char *)_dev->name, (const char *)name, 8);
    _dev->trigger_level = _trigger_level;
    _dev->long_time = _long_time;
    p1->next = _dev;
    _dev->next = NULL;

    return 0;
}

/**
 * @brief 连接IO回调函数
 *
 * @param _dev 设备对象
 * @param _io IO回调列表
 * @param _cb IO回调函数
 * @return uint8_t
 */
uint8_t tl_btn_io_callback_attach(tl_button_t *_dev, const btn_io_cb_t _cb)
{
    // 检查参数是否合理
    if (_dev == NULL)
    {
        return 1;
    }

    // 绑定回调函数
    _dev->io_cb = _cb;

    return 0;
}

/**
 * @brief 连接事件回调函数
 *
 * @param _dev 设备对象
 * @param _event 事件回调列表
 * @param _cb 事件回调函数
 * @return uint8_t
 */
uint8_t tl_btn_event_callback_attach(tl_button_t *_dev, const uint8_t _event, const btn_event_cb_t _cb)
{
    // 检查参数是否合理
    if ((_dev == NULL) || (_event >= eTL_BTN_EVENT_CB_MAX))
    {
        return 1;
    }

    // 绑定回调函数
    _dev->event_cb[_event] = _cb;

    return 0;
}

/**
 * @brief 编码器对象解析方法
 *
 * @param _dev 设备对象
 */
void tl_btn_io_scan(tl_button_t *_dev)
{
    // 获取当前IO电平状态
    if (NULL == _dev->io_cb)
    {
        _dev->state = tl_btn_io_callback_handler();
    }
    else
    {
        _dev->state = _dev->io_cb();
    }
}

void tl_btn_event_resolution(tl_button_t *_dev)
{
    // 基本标记（按下，松开）
    // 按键扫描
    // 这里判断两次时间戳的状态，判断触发电平后设置按键事件
    if ((_dev->last_state != _dev->state))
    {
        // 判断按键状态是否是触发电平
        if (_dev->state == _dev->trigger_level)
        {
            // 设置按键按下回调标记
            tl_btn_callback_flag_set(_dev, TL_BTN_DOWN_FLAG);
        }
        else
        {
            // 设置按键松开回调标记
            tl_btn_callback_flag_set(_dev, TL_BTN_UP_FLAG);
        }
    }

    // 扩展标记（短按，长按，连击）
    // 按键松开
    if (TL_BTN_UP_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_UP_FLAG))
    {
        if (_dev->trigger_time < _dev->long_time)
        {
            _dev->multiple++;
            if (_dev->multiple < 2)
            {
                // 设置短按回调标记
                tl_btn_callback_flag_set(_dev, TL_BTN_CLICK_FLAG);
            }
            else
            {
                // 设置连按回调标记
                tl_btn_callback_flag_set(_dev, TL_BTN_MULTIPLE_FLAG);
            }
        }
        _dev->trigger_time = 0;
    }
    // 按键按下
    if (TL_BTN_DOWN_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_DOWN_FLAG))
    {
        _dev->reset_time = 500;
        _dev->trigger_time++;
        if (_dev->trigger_time >= _dev->long_time)
        {
            _dev->trigger_time = _dev->long_time;
            // 设置长按回调标记
            tl_btn_callback_flag_set(_dev, TL_BTN_LONG_FLAG);
        }
    }

    if (_dev->reset_time == 0)
    {
        // _dev->trigger_time = 0;
        _dev->multiple = 0;
    }
    else
    {
        _dev->reset_time--;
    }

    // 比较前后时间戳的电平状态(备份上一次时间戳的电平状态)
    // key
    if ((_dev->last_state != _dev->state))
    {
        _dev->last_state = _dev->state;
    }
}

/**
 * @brief 编码器回调操作
 *
 * @param _dev 设备对象
 */
void tl_btn_callback_ops(tl_button_t *_dev)
{
    // 按键松开
    if (TL_BTN_UP_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_UP_FLAG))
    {
        if (NULL == _dev->event_cb[eTL_BTN_EVENT_CB_KEY_UP])
        {
            tl_btn_up_callback_handler();
        }
        else
        {
            _dev->event_cb[eTL_BTN_EVENT_CB_KEY_UP]();
        }
    }
    // 按键按下
    if (TL_BTN_DOWN_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_DOWN_FLAG))
    {
        if (NULL == _dev->event_cb[eTL_BTN_EVENT_CB_KEY_DOWN])
        {
            tl_btn_down_callback_handler();
        }
        else
        {
            _dev->event_cb[eTL_BTN_EVENT_CB_KEY_DOWN]();
        }
    }
    // 按键短按
    if (TL_BTN_CLICK_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_CLICK_FLAG))
    {
        if (NULL == _dev->event_cb[eTL_BTN_EVENT_CB_KEY_CLICK])
        {
            tl_btn_click_callback_handler();
        }
        else
        {
            _dev->event_cb[eTL_BTN_EVENT_CB_KEY_CLICK]();
        }
    }
    // 按键长按
    if (TL_BTN_LONG_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_LONG_FLAG))
    {
        if (NULL == _dev->event_cb[eTL_BTN_EVENT_CB_KEY_LONG])
        {
            tl_btn_long_callback_handler();
        }
        else
        {
            _dev->event_cb[eTL_BTN_EVENT_CB_KEY_LONG]();
        }
    }
    // 按键连击
    if (TL_BTN_MULTIPLE_FLAG == tl_btn_callback_flag_get(_dev, TL_BTN_MULTIPLE_FLAG))
    {
        if (NULL == _dev->event_cb[eTL_BTN_EVENT_CB_KEY_MULTIPLE])
        {
            tl_btn_multiple_callback_handler();
        }
        else
        {
            _dev->event_cb[eTL_BTN_EVENT_CB_KEY_MULTIPLE]();
        }
    }
}

/**
 * @brief 读取连击次数
 *
 * @param _dev 设备对象
 * @return uint8_t 连击次数
 */
uint8_t tl_btn_multiple_read(const tl_button_t *_dev)
{
    return _dev->multiple;
}

/**
 * @brief 设置回调标记
 *
 * @param _dev 设备对象
 * @param _callback_flag 回调标记
 */
void tl_btn_callback_flag_set(tl_button_t *_dev, const uint16_t _callback_flag)
{
    _dev->callback_flag |= _callback_flag;
}

/**
 * @brief 获取回调标记
 *
 * @param _dev 设备对象
 * @param _callback_flag 回调标记
 * @return uint16_t
 */
uint16_t tl_btn_callback_flag_get(tl_button_t *_dev, const uint16_t _callback_flag)
{
    return (_dev->callback_flag & _callback_flag);
}

/**
 * @brief 清除回调标记
 *
 * @param _dev 设备对象
 * @param _callback_flag 回调标记
 */
void tl_btn_callback_flag_clear(tl_button_t *_dev, const uint16_t _callback_flag)
{
    _dev->callback_flag &= (~_callback_flag);
}

/**
 * @brief 编码器的进程函数
 *
 */
void tl_btn_process(void)
{
    tl_button_t *p1 = btn_head.next;

    // 遍历整个链表
    for (; p1 != NULL; p1 = p1->next)
    {
        // 单个对象解析
        tl_btn_io_scan(p1);
        tl_btn_event_resolution(p1);
        tl_btn_callback_ops(p1);
    }
}
