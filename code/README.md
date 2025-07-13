+-------------------+
|   app_main()      |
+-------------------+
         |
         v
+-------------------+
| state_machine_run |
+-------------------+
         |
         v
+-------------------+
| ui_service_send_event |
+-------------------+
         |
         v
+-------------------+     +-------------------+
|  ui_refresh_task  | --> | ui_service_handle_event |
+-------------------+     +-------------------+
         |                        |
         v                        v
+-------------------+     +-------------------+
| wifi_service_*    |     | view_* functions  |
+-------------------+     +-------------------+


## 防止撕裂 https://blog.csdn.net/BoRuiYiQi/article/details/138170707

一、硬件配置优化
‌降低PCLK时钟频率‌
过高的像素时钟会导致PSRAM带宽不足，引发画面漂移。建议根据屏幕规格降低pclk值，例如调整为10MHz或更低13。
‌增强PSRAM与Flash带宽‌
设置Flash为QIO 120MHz模式
配置PSRAM为Octal 120MHz模式
提升存储带宽可缓解数据传输瓶颈1。
‌电源噪声抑制‌
为屏幕和ESP32独立供电，并在电源引脚并联滤波电容（如100μF电解电容+0.1μF陶瓷电容），避免电压波动导致花屏3。
二、软件参数调整
‌时序配置优化‌
修改LCD驱动时序结构体参数，关键值需严格匹配屏幕手册：
c
Copy Code
lcd_panel_dev_config_t panel_config = {
  .clk_freq_hz = 10 * 1000 * 1000,  // 降低像素时钟
  .hsync_back_porch = 50,           // 增加水平后廊
  .vsync_back_porch = 20,           // 增加垂直后廊
  // 其他参数按屏幕手册调整
};
增大hsync_back_porch和vsync_back_porch可缓解撕裂3。
‌启用垂直同步复位‌
开启CONFIG_LCD_RGB_RESTART_IN_VSYNC选项，强制在VSYNC信号期间刷新帧数据（可能轻微降低帧率）1。
三、系统级优化
‌编译器与缓存配置‌
启用CONFIG_COMPILER_OPTIMIZATION_PERF性能优化
降低data_cache_line_size至32字节
# 开启CONFIG_SPIRAM_FETCH_INSTRUCTIONS和CONFIG_SPIRAM_RODATA以加速PSRAM访问1。
‌信号完整性措施‌
缩短RGB排线长度（建议<15cm）
为数据线添加100Ω终端电阻，减少信号反射3。
四、其他关键点
‌避免高干扰环境‌：远离电机、继电器等设备，必要时为屏幕加装屏蔽罩3。
‌更新ESP-IDF驱动‌：使用最新版LCD驱动库，修复旧版本的时序兼容性问题3。
‌优先执行顺序‌：先降频PCLK并优化时序 → 增强电源滤波 → 调整PSRAM配置 → 检查物理连接。若仍存在撕裂，可尝试牺牲帧率启用VSYNC复位