# mdk_project_template

## 概述
MDK(KEIL5)工程模板。
- SingleProject
    单工程模板。

- MultiProject
    项目模板，同一个硬件平台下使用相同驱动，包含多个工程（多个应用）。

工程包含组件部分和应用部分，组件部分按硬件层次分离，层内按外设对象分离。

- 应用层
    - 应用1
    - 应用2
- 组件层
    - 扩展（板载）外设功驱动
    - 芯片外设驱动
    - 硬件抽象层（MCU lib）
            
## 文件目录

#### SingleProject
- app ---- 应用目录
- doc ---- 文档目录
- lib ---- 外部库目录
- MDK ---- MDK工程目录
- module ---- 组件部分目录
    - EPL ---- 扩展外设驱动
    - HAL ---- 硬件抽象目录
    - MPL ---- 芯片外设驱动
- tools ---- 工具目录

#### MultiProject
- doc ---- 项目文档目录
- lib ---- 外部库目录
- project ---- 项目目录
    - common ---- 公共文件目录
        - HAL ---- 硬件抽象层
        - MPL ---- 芯片外设目录
    - template1 ---- 工程1目录
        - app ---- 应用层目录
        - MDK ---- MDK工程目录
        - EPL ---- 扩展外设目录
    - template2 ---- 工程2目录
        - app ---- 应用层目录
        - MDK ---- MDK工程目录
        - EPL ---- 扩展外设目录
    ...
- tools ---- 工具目录