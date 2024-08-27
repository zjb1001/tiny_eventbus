# Tiny EventBus 使用说明

## 概述

Tiny EventBus 是一个轻量级的、基于共享内存和信号量的进程间事件总线系统。它允许不同的进程注册为发布者或订阅者，并通过共享内存进行消息的发布和订阅。

## 功能

- **发布者注册**：进程可以注册为发布者，并获得一个唯一的发布者ID。
- **消息发布**：发布者可以通过其ID发布消息到指定的主题。
- **订阅者注册**：进程可以订阅感兴趣的主题，并接收相关消息。
- **消息订阅**：订阅者可以接收并处理发布到其订阅主题的消息。

## 基本框架介绍

### 目录结构

```
/home/pg/OwnRepoground/tiny_eventbus/
├── Makefile
├── src/
│   ├── eventbus.h
│   ├── eventbus.c
│   ├── publisher.c
│   └── subscriber.c
```

### 主要文件说明

- **eventbus.h**：定义了事件总线的数据结构和API接口。
- **eventbus.c**：实现了事件总线的核心功能，包括共享内存和信号量的初始化、发布者和订阅者的注册、消息的发布和订阅等。
- **publisher.c**：实现了发布者的主程序，允许进程注册为发布者并发布消息。
- **subscriber.c**：实现了订阅者的主程序，允许进程订阅主题并接收消息。
- **Makefile**：用于编译和构建项目的Makefile文件。

### 编译和运行

1. **编译项目**：
    ```bash
    make
    ```

2. **运行发布者**：
    ```bash
    ./publisher <publisher_name>
    ```

3. **运行订阅者**：
    ```bash
    ./subscriber <topic_name>
    ```

## 示例

### 发布者示例

```bash
./publisher my_publisher
```

### 订阅者示例

```bash
./subscriber my_topic
```

## 贡献

欢迎贡献代码和提出改进建议。请通过GitHub提交Pull Request或Issue。

## 许可证

本项目采用MIT许可证。详细信息请参阅LICENSE文件。