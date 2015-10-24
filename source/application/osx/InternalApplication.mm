//
// Created by Walter Rawdanik on 10/20/15.
//
#include "InternalApplication.h"
#include "Application.h"
#import <AppKit/NSApplication.h>
#import <AppKit/NSAlert.h>
#include "MiniEngineApplicationDelegate.h"

using namespace MiniEngine;

InternalApplication::InternalApplication(Application *app)
{
    mApplication=app;
}
InternalApplication::~InternalApplication()
{

}
void InternalApplication::runApplication(int args, char *argv[])
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    [NSApplication sharedApplication];

    MiniEngineApplicationDelegate *delegate=[MiniEngineApplicationDelegate new];

    delegate.application=this;


    [NSApp setDelegate: delegate];
    [[NSApplication sharedApplication] run];


    [pool drain];
}
void InternalApplication::destroyApplication()
{

}
void InternalApplication::frameStarted()
{

}
void InternalApplication::frameEnded()
{

}

void InternalApplication::run()
{

}

RenderTargetPtr InternalApplication::createDefaultTarget()
{
    return RenderTargetPtr();
}