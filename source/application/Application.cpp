//
// Created by Walter Rawdanik on 10/19/15.
//

#include "Application.h"
#include "Log.h"
#include "InternalApplication.h"

using namespace MiniEngine;



Application::Application()
{

}

Application::~Application()
{

}

void Application::onInit(RenderManager *manager)
{

}
void Application::onUpdate(FrameTime &frameTime)
{
    Log("Entered update");

}
void Application::onActivate()
{

}
void Application::onExit()
{

}
void Application::onSuspend()
{

}
void Application::onResume()
{

}
void Application::onOrientationChange()
{

}

void Application::setInternalApplication(InternalApplication *app)
{
    mInternalApp=app;
}

void Application::runApplication(Application *app,int argc, char *argv[])
{
    if(app!=nullptr)
    {
        InternalApplication *internalApp=new InternalApplication(app);
        app->setInternalApplication(internalApp);
        internalApp->runApplication(argc,argv);
    }
}