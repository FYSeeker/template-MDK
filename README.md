# KEIL5ProjectTemplate-STM32
## 概述
KEIL5(MDK)工程模板，用于STM32系列单片机。
- SingleProject
工程模板。
- MultiProject
项目模板。同一芯片，包含多个工程（多个应用）。

工程包含组件部分和应用部分，组件部分按硬件层次分离，层内按外设对象分离。
- 应用层
- 组件层
    - 外设功能层
        - 硬件抽象层
            
## 文件目录
#### SingleProject
- app ---- 应用层目录
- doc ---- 文档目录
- lib ---- 外部库目录
- MDK ---- MDK工程目录
- module ---- 组件部分目录
    - APL ---- 应用组件层
    - EPL ---- 扩展外设层
    - HAL ---- 硬件抽象层
    - MPL ---- 芯片外设层
- tools ---- 工具目录
#### MultiProject
- doc ---- 项目文档目录
- lib ---- 外部库目录
- project ---- 项目目录
    - common ---- 公共文件目录
        - HAL ---- 硬件抽象层
    - template1 ---- 工程1目录
        - app ---- 应用层目录
        - MDK ---- MDK工程目录
        - EPL ---- 扩展外设目录
        - MPL ---- 芯片外设目录
    - template2 ---- 工程2目录
    - template3 ---- 工程3目录
    - ...
    - templateN ---- 工程N目录
- tools ---- 工具目录