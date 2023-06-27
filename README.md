# 前言



本仓库是基于PC端的微信逆向源码，用于记录与分享一下自己的学习过程

微信版本：3.6.0.18

> 如果对你有帮助，别吝啬你的小手留个star呗



### 源码的编译环境

使用vs2019选择x86环境，基本上都能一次编译过

嫌麻烦的话，直接自己创建一个项目，拷贝dllmain.cpp的内容即可



### 成品的使用教程

> 使用前请先安装指定版本的微信
> 指定版本微信安装包：https://pan.baidu.com/s/1dxBuvDgAeI0mFjGsY6NVNA
> 提取码：sszs

- 文档地址：https://www.showdoc.com.cn/2228290215170955

1. 运行《注入工具(图形界面版).exe》点击启动并注入
2. 监听消息：开启一个HTTP服务，并且运行在 `9000` 端口上，请求路径为 `/msg`，请求方法为 `POST`
3. 操作接口：注入成功后，即可向 `8989` 端口发送HTTP请求，执行对应操作，请求路径为 `/api`，请求方法为 `POST`



### 更灵活的使用方法

如需更灵活的使用，可使用命令行调用《inject_tool.exe》

命令参数：[inject_tool.exe的全路径] start [DLL的端口号]

示例：`C:\inject_tool.exe start 8989 --my_port=9000`

注意：`--my_port` 就是你用来接收消息的端口号，这个参数是可选的，不填的话默认就是9000



### 疑问解答、学习交流

- 交流QQ群：54995858



### 声明

**本项目仅供技术研究，请勿用于非法用途，如有任何人凭此做何非法事情，均于作者无关，特此声明。**