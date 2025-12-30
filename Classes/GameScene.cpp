#include "GameScene.h"
#include "ui/CocosGUI.h"
#include "models/UndoModel.h" 
#include "views/GameView.h"    
#include "models/GameModel.h"  
#include "controllers/GameController.h"

USING_NS_CC;

// GameScene类
// 功能：游戏主场景核心
// 职责：初始化GameModel数据模型与UndoManager撤销管理器;关联GameController，绑定触摸监听;初始化GameView;管理场景内动态资源的内存释放
// 使用场景：游戏启动后由AppDelegate加载的核心场景

 // 构造函数
 // 功能：初始化GameScene，创建数据模型与撤销管理器，初始化私有成员

GameScene::GameScene() {
    _gameModel = new GameModel();
    _undo = new UndoManager(_gameModel);
    _handTop = nullptr;
}

// 析构函数
// 功能：销毁GameScene，释放所有动态分配资源

GameScene::~GameScene() {
    delete _undo;
    delete _gameModel;
    for (auto card : _tableCards) {
        if (card) {
            card->removeFromParent();
        }
    }
    _tableCards.clear();
}

// 功能：创建并返回GameScene场景

Scene* GameScene::createScene() {
    return GameScene::create();
}

// 功能：初始化游戏场景
bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }

    auto rootLayer = Layer::create();
    this->addChild(rootLayer, -1);

    auto gameModel = new GameModel();

    auto gameView = new GameView(rootLayer, gameModel);
    gameView->initView();

    auto gameController = new GameController(gameModel, gameView, _undo);
    gameView->setController(gameController);
    gameController->initTouchListener(rootLayer);

    return true;
}

// 功能：卡牌点击事件

void GameScene::onCardClicked(CardView* card) {

}