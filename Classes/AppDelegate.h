#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

// AppDelegate类
// 功能：游戏入口
// 职责：初始化OpenGL,游戏启动
// 使用场景：全局配置

class AppDelegate : private cocos2d::Application
{
public:
    // 构造函数
    // 功能：初始化AppDelegate实例

    AppDelegate();

    // 功能：销毁AppDelegate实例

    virtual ~AppDelegate();

    // 功能：配置GL
    virtual void initGLContextAttrs();

    // 功能：游戏启动，完成窗口创建、分辨率适配、主场景加载
    virtual bool applicationDidFinishLaunching();

    // 功能：暂停游戏动画
    virtual void applicationDidEnterBackground();

    // 功能：恢复游戏动画

    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_