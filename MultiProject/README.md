# Talos外接传感器程序
## 概述
talos机器人胸前，外接传感器的程序。
## 协议
moto型号为0x1d,sensor型号为0x0c。
## 传感器
|传感器|ID|数据地址|权限|说明|
|:-:|:-:|:-:|:-:|:-:|
|illuminance module|100|0x1a|可读|存储着光线强度信息|
|humiture module|101|0x1a-0x1d|可读|分别为温度整数、温度小数、湿度整数、湿度小数|
|infrared module|102|0x1a|可读|0x01检测到，0x00没检测到|
|color module|103|0x1a|可读|存储着颜色信息|
|fire module|104|0x1a|可读|0x01检测到，0x00没检测到|
|fan module|105|0x1a|可写|0x10-0x14代表五个转速，正转;0x20-0x24代表五个转速，反转;其余值不转|
|gas module|106|0x1a|可读|表示酒精浓度值|
|touch modeule|107|0x1a|可读|-|