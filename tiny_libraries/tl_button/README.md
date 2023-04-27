# 如何使用？

## 1.用户自行配置好GPIO

## 2.使用默认回调函数
> tl_button_callback.c
``` c
__WEAK uint8_t tl_btn_io_callback_handler(void)
{
    return 0;
}

__WEAK void tl_btn_up_callback_handler(void)
{

}

__WEAK void tl_btn_down_callback_handler(void)
{
    
}

__WEAK void tl_btn_click_callback_handler(void)
{
    
}

__WEAK void tl_btn_long_callback_handler(void)
{
    
}

__WEAK void tl_btn_multiple_callback_handler(void)
{
    
}
```
> 或者自定义回调函数
``` c
uint8_t key_io_cb(void)
{
    return gpio_input_data_bit_read(GPIOA, GPIO_PINS_2);
}
void up_event_cb(void)
{
  if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_UP_FLAG))
  {
    tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_UP_FLAG);
    ec1_key_up++;
  }
  if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_DOWN_FLAG))
  {
    tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_DOWN_FLAG);
    // ec1_key_down++;
  }
}
void down_event_cb(void)
{
  // if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_DOWN_FLAG))
  {
    // tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_DOWN_FLAG);
    ec1_key_down++;
  }
}
void click_event_cb(void)
{
  if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_CLICK_FLAG))
  {
    tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_CLICK_FLAG);
    ec1_key_click++;
  }
}
void long_event_cb(void)
{
  if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_LONG_FLAG))
  {
    tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_LONG_FLAG);
    ec1_key_long++;
  }
}
void multiple_event_cb(void)
{
  if (tl_btn_callback_flag_get(&ec1_btn, TL_BTN_MULTIPLE_FLAG))
  {
    tl_btn_callback_flag_clear(&ec1_btn, TL_BTN_MULTIPLE_FLAG);
    ec1_key_mul = tl_btn_multiple_read(&ec1_btn);
  }
}
```

## 3.定义对象
``` c
tl_button_t ec1_btn;
```

## 4.初始化
``` c
  // 创建对象
  tl_btn_create("ec1 btn", &ec1_btn, eTL_BTN_IO_LV_LOW, 2000);
  // 绑定IO的操作函数（读取）
  tl_btn_io_callback_attach(&ec1_btn, key_io_cb);
  // 绑定事件回调函数
  tl_btn_event_callback_attach(&ec1_btn, eTL_BTN_EVENT_CB_KEY_UP, up_event_cb);
  tl_btn_event_callback_attach(&ec1_btn, eTL_BTN_EVENT_CB_KEY_DOWN, down_event_cb);
  tl_btn_event_callback_attach(&ec1_btn, eTL_BTN_EVENT_CB_KEY_CLICK, click_event_cb);
  tl_btn_event_callback_attach(&ec1_btn, eTL_BTN_EVENT_CB_KEY_LONG, long_event_cb);
  tl_btn_event_callback_attach(&ec1_btn, eTL_BTN_EVENT_CB_KEY_MULTIPLE, multiple_event_cb);
```

## 5.进程(主循环或者定时器-1ms)
``` c
tl_btn_process();
```
