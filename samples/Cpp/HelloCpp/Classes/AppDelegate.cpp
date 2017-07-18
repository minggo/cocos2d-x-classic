#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "LoadingScene.h"
#include "AppMacros.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxEngineDataManager.h"
#endif

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

class MyTouchDelegate : public CCObject, public CCTouchDelegate
{
public:

    MyTouchDelegate() {}

    virtual ~MyTouchDelegate()
    {
    }

    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
    {
        CCTouch *touch = (CCTouch*)pTouches->anyObject();
        CCPoint location = touch->getLocation();

        if (location.x > 200 && location.x < 280 && location.y < 100)
        {
            CCLog("ccTouchesEnded, EngineDataManager::disable()");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            EngineDataManager::disable();
#endif
        }
    }
 };

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);

	CCSize frameSize = pEGLView->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *mainScene = HelloWorld::scene();

    CCScene* pScene = LoadingScene::create(mainScene);

    // run
    pDirector->runWithScene(pScene);

    CCTouchDispatcher* dispatcher = pDirector->getTouchDispatcher();
    MyTouchDelegate* delegate = new MyTouchDelegate();
    dispatcher->addStandardDelegate(delegate, 0);
    delegate->release();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
