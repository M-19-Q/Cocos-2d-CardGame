// AppDelegate类
// 功能：控制文件
// 职责：游戏启动、初始化分辨率、加载游戏主场景
// 使用场景：游戏入口

#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

// 常量：游戏设计分辨率
static const cocos2d::Size kDesignResolutionSize = cocos2d::Size(1080, 2080);
// 常量：预设小分辨率
static const cocos2d::Size kSmallResolutionSize = cocos2d::Size(480, 320);
// 常量：预设中分辨率
static const cocos2d::Size kMediumResolutionSize = cocos2d::Size(1024, 768);
// 常量：预设大分辨率
static const cocos2d::Size kLargeResolutionSize = cocos2d::Size(2048, 1536);

// 构造函数
// 功能：初始化AppDelegate实例

AppDelegate::AppDelegate()
{
}


// 析构函数
// 功能：销毁AppDelegate实例

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 功能：初始化OpenGL

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

// 功能：扩展接口，用于注册自定义类、资源包等

static int registerAllPackages()
{
    return 0;
}


// 功能：游戏启动，完成窗口创建、分辨率适配、主场景加载

bool AppDelegate::applicationDidFinishLaunching() {
    // 获取核心渲染/场景控制器
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();

    // 未创建GL视图时，根据平台创建窗口/视图
    if (!glView) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // 桌面平台创建固定尺寸窗口（标题：test，尺寸：设计分辨率）
        glView = GLViewImpl::createWithRect("test", cocos2d::Rect(0, 0, kDesignResolutionSize.width, kDesignResolutionSize.height));
#else
        // 移动平台自动适配屏幕创建视图
        glView = GLViewImpl::create("test");
#endif
        director->setOpenGLView(glView);
    }

    // 启用性能统计
    director->setDisplayStats(true);
    // 设置动画帧率（60FPS）
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率及适配策略（SHOW_ALL：等比缩放，显示全部）
    glView->setDesignResolutionSize(kDesignResolutionSize.width, kDesignResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    // 获取设备物理屏幕尺寸
    auto frameSize = glView->getFrameSize();

    // 根据设备屏幕尺寸动态计算缩放比例，适配不同分辨率
    if (frameSize.height > kMediumResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(kLargeResolutionSize.height / kDesignResolutionSize.height, kLargeResolutionSize.width / kDesignResolutionSize.width));
    }
    else if (frameSize.height > kSmallResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(kMediumResolutionSize.height / kDesignResolutionSize.height, kMediumResolutionSize.width / kDesignResolutionSize.width));
    }
    else
    {
        director->setContentScaleFactor(MIN(kSmallResolutionSize.height / kDesignResolutionSize.height, kSmallResolutionSize.width / kDesignResolutionSize.width));
    }

    // 注册自定义包
    registerAllPackages();
    // 创建游戏主场景并运行
    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

// 功能：处理应用后台事件（暂停游戏动画、暂停音频）

void AppDelegate::applicationDidEnterBackground() {
    // 暂停游戏动画渲染/更新
    Director::getInstance()->stopAnimation();
}


// 功能：恢复游戏动画

void AppDelegate::applicationWillEnterForeground() {
    // 恢复游戏动画渲染/更新
    Director::getInstance()->startAnimation();
}