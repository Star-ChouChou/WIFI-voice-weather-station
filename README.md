# XYD-WIFI-voice-weather-station
WIFI Voice Weather Station base on stm32f103zet6
基于stm32f103zet6的WIFI语音气象站
通过ESP8266开机自动校正时间，获取天气情况
通过DHT11温湿度模块、MQ2获取空气质量、光敏电阻获取光照强度
实现按键控制语音播报当天天气和未来天气
LCD屏幕通过LVGL实现页面的设计显示，显示当前室内实时温湿度、光照强度、空气质量
按键控制切换屏幕页面，显示未来三天天气，按钮控制自动播报天气
显示全国主要城市天气，按键实现切换
