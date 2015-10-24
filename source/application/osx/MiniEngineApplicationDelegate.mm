

#include "Log.h"
#import <AppKit/NSMenuItem.h>
#import <AppKit/NSMenu.h>
#include <string.h>
#import <AppKit/NSAlert.h>

#import "MiniEngineApplicationDelegate.h"
#include "RenderManager.h"
#include "Application.h"

@implementation MiniEngineApplicationDelegate

@synthesize application;


- (id)init 
{
    using namespace MiniEngine;
    
	self = [super init];
	return self;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{     
				
    using namespace MiniEngine;

    [[NSFileManager defaultManager] changeCurrentDirectoryPath: [[NSBundle mainBundle] resourcePath]];

    //Application *app=Application::instance();
    //ApplicationOSX *iApp=(ApplicationOSX*)app->internalApplication();
    //ApplicationParameters &params=app->parameters();
    //iApp->initApplication(&params);
    //iApp->setState(ApplicationState::Running);
       

    NSApplication *appo = [notification object];
    NSMenu *appleMenu = [[NSMenu alloc] initWithTitle:@"Apple Menu"];
    [appleMenu addItemWithTitle:@"About" action:@selector(_aboutMenu:) keyEquivalent:@""];
    [appleMenu addItemWithTitle:@"Caps" action:@selector(_capsMenu:) keyEquivalent:@""];    
    [appleMenu addItem:[NSMenuItem separatorItem]];
    [appleMenu addItemWithTitle:@"Switch Orientation" action:@selector(_orientMenu:) keyEquivalent:@""];        
    [appleMenu addItem:[NSMenuItem separatorItem]];
    [appleMenu addItemWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
    NSMenuItem * menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
    NSMenu *mainMenu = [[NSMenu alloc] initWithTitle: @"" ];
    [menuItem setSubmenu:appleMenu];
    [mainMenu addItem:menuItem];
    [appo setMainMenu:mainMenu];
    [appo setAppleMenu:appleMenu];
    [menuItem release];
    [mainMenu release];
    [appleMenu release];    
    
    
    [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0) target:self selector:@selector(_applicationLoop) userInfo:nil repeats:YES];

    //iApp->run();
    
    return ;
	
}

- (void)applicationWillTerminate:(NSNotification *)notification 
{
    using namespace MiniEngine;

/*
    Application *app=Application::instance();
    ApplicationOSX *iApp=(ApplicationOSX*)app->internalApplication();
    iApp->setState(ApplicationState::Terminated);
    iApp->destroyApplication();
*/
}


-(void)dealloc {
	
	[super dealloc];
}


-(void)_applicationLoop
{
	using namespace MiniEngine;


    _Log("app is runnig\n");

	//ApplicationOSX *app=(ApplicationOSX*)Application::instance()->internalApplication();
    //app->run();
}

-(void)_aboutMenu:(id)sender
{
    const char *title="About MiniEnigine iOS Emulator";
    rde::string message;
    
	using namespace MiniEngine;

/*
    RenderManager *manager=RenderManagerFactory::instance()->manager();
    if(manager)
    {
        RenderSystem *rSystem=manager->renderSystem();
        if(rSystem)
        {
            const Capabilities& caps=rSystem->capabilities();
            
            message.sprintf("(c) 2008 Walter Rawdanik\n\nRenderer:%s\n\nVendor:%s:\n\nVersion:%s\n\nShading Language:%s\n\n",
                            caps.renderer().c_str(),caps.vendor().c_str(),caps.version().c_str(),caps.shadingLanguage().c_str());            
            
        }
        
    }    
    
    Application *app=Application::instance();
    ApplicationOSX *iApp=(ApplicationOSX*)app->internalApplication();
    
    iApp->displaySystemMsg(title, message.c_str());
*/
}

-(void)_capsMenu:(id)sender
{
    const char *title="Driver Caps";    
    rde::string message;
    
	using namespace MiniEngine;

/*
    RenderManager *manager=RenderManagerFactory::instance()->manager();
    if(manager)
    {
        RenderSystem *rSystem=manager->renderSystem();
        if(rSystem)
        {
            const Capabilities& caps=rSystem->capabilities();
            
            rde::string tmp;
            
            tmp.sprintf("Max Viewport Size: ( %d,%d )\nTexture Units: %d\nMax Texture Size: %d\nVertex Attributes: %d\nVertex Uniform Vectors: %d\nFragment Uniform Vectors: %d\nVarying Vectors:  %d\nMax Anisotropy Filter: %f\n",                            
                            (int)caps.maxViewportSize().x,(int)caps.maxViewportSize().y,caps.maxTextureUnits(),caps.maxTextureSize(),caps.maxVertexAttributes(),
                            caps.maxVertexUniformVectors(),caps.maxFragmentUniformVectors(),caps.maxVaryingVectors(),caps.maxAnisotropicFilter());            
                                    
            message.append(tmp.c_str(), tmp.length());
                        
            const GLubyte *  extensions = glGetString(GL_EXTENSIONS);                        
            
            if(extensions!=0)
            {
                unsigned int index=0;

                rde::string extString;
                
                while(extensions[index]!=0)
                {
                    if(extensions[index]==32)
                        extString.append("\n", 1);
                    else
                        extString.append((const char*)&extensions[index], 1);                                            
                       ++index;
                }                            
                
                message.append("\n", 1);
                message.append(extString);                
            }
            
            
        }
        
    }    
    
    Application *app=Application::instance();
    ApplicationOSX *iApp=(ApplicationOSX*)app->internalApplication();
    
    iApp->displaySystemMsg(title, message.c_str());
*/

}

-(void)_orientMenu:(id)sender
{
	using namespace MiniEngine;

/*
    RenderManager *manager=RenderManagerFactory::instance()->manager();
    if(manager)
    {
        RenderTargetWindowOSX *windowTarget=(RenderTargetWindowOSX*)manager->defaultTarget();
        if(windowTarget)
        {
            
            const Viewport &view=windowTarget->viewport();
            
            NSWindow *nsWindow=(NSWindow*)windowTarget->windowData().mAppWindow;
            
            NSView *nsView=(NSView*)windowTarget->windowData().mAppView;
            
            NSRect frame3=[nsView frame];
            
            _Log("old VIEW %f,%f,%f,%f,",frame3.origin.x,frame3.origin.y, frame3.size.width, frame3.size.height);            
            
            NSRect frame=[nsWindow frame];
            
            _Log("got frame %f,%f,%f,%f,",frame.origin.x,frame.origin.y, frame.size.width, frame.size.height);
            
            NSRect frameFinal = [nsWindow frameRectForContentRect: NSMakeRect(frame.origin.x, frame.origin.y,
                                                                               view.height(), view.width())];
            
            _Log("new frame %f,%f,%f,%f,",frame.origin.x,frame.origin.y, frame.size.width, frame.size.height);
            
            [nsWindow setFrame:frameFinal display:NO animate:NO];
            
            windowTarget->setDefaultViewport(Viewport(0,0,view.height(),view.width()));
            
            
            NSRect frame2=[nsView frame];
            
            _Log("new VIEW %f,%f,%f,%f,",frame2.origin.x,frame2.origin.y, frame2.size.width, frame2.size.height);

            Application *app=Application::instance();
            ApplicationOSX *iApp=(ApplicationOSX*)app->internalApplication();
            
            
            RenderingContextOSX *osx=(RenderingContextOSX*)windowTarget->renderingContext();
            
            
            EGLint ai32ContextAttribs[16];
            int	i = 0;
            
            ai32ContextAttribs[i++] = EGL_CONTEXT_CLIENT_VERSION;
            ai32ContextAttribs[i++] = 2;                
            ai32ContextAttribs[i] = EGL_NONE;
            
            EGLContext oldContext=osx->mContext;
            
            //osx->mContext = eglCreateContext(osx->mEGLDisplay, osx->mEGLConfig, NULL, ai32ContextAttribs);            
            
            osx->create(windowTarget->parameters(),windowTarget->windowData().mAppView);

            RenderStateGLES2 *rState=(RenderStateGLES2*)manager->renderSystem()->renderState();
            

            
            app->init(*windowTarget->parameters(), manager);

            rState->resetToDefault();            
            
            iApp->addOrientationChange(OrientationChange(ScreenRotation::Rotation180,ScreenRotation::Rotation0,ScreenRotation::End));
            
            //glFlush();
            
        }
                                        
        
    }    
    */
}

@end


@implementation AppWindow

@synthesize application;

- (id)init 
{
    using namespace MiniEngine;
    
    application=nil;
	self = [super init];
	return self;
}


- (void)keyDown:(NSEvent*) event
{
/*
    if(application)
    {
        // Handle the keyboard input
        switch([event keyCode])
        {

        }
    }
    */
}

- (void)windowWillClose:(NSNotification *)notification
{
    using namespace MiniEngine;
/*
if(application)
    {
        application->applicationData()->mNeedsToQuit=true;

    }
    */
    [NSApp terminate:nil];
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
    using namespace MiniEngine;
/*
    if(application)
    {
        application->setState(ApplicationState::Paused);        
    }
    */
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
    using namespace MiniEngine;

/*
    if(application)
    {
        application->setState(ApplicationState::Running);        
    }
    */
}


- (void)mouseDown:(NSEvent*)event
{
    using namespace MiniEngine;

/*
    if(application)
    {
        switch([event type])
        {
                
            case NSLeftMouseDown:
            {                
                NSPoint location = [event locationInWindow];
                                                               
                NSRect  rect = [NSWindow contentRectForFrameRect: [self frame] styleMask: [self styleMask]];                                
                
                eventId=[event eventNumber];
                
                int offsetY=0;
                
                if([event modifierFlags]&NSControlKeyMask)
                    offsetY=50;
                
                MouseEventArray &events=application->mouseEvents();                
                events.push_back(MouseInput(MouseState::Down,(int)(location.x),
                                            (int)(rect.size.height -(location.y))-offsetY,0, (int)eventId,(unsigned long)([event timestamp]*1000.0)));  
                
                
                
                if([event modifierFlags]&NSControlKeyMask)
                {
                    
                    pinchEventId=eventId+10;
                    trackingPinch=true;
                    pinchPosX=(int)(location.x);
                    pinchPosY=(int)(rect.size.height -(location.y))+offsetY;
                    events.push_back(MouseInput(MouseState::Down,pinchPosX,
                                                pinchPosY,0, pinchEventId,(unsigned long)([event timestamp]*1000.0)));                    
                    
                }
                
                
            }
                break;

            default: {}
        }
    }
    */
}

- (void)mouseUp:(NSEvent*)event
{
/*
    if(application)
    {
        switch([event type])
        {
            using namespace MiniEngine;
                
            case NSLeftMouseUp:
            {                
                NSPoint location = [event locationInWindow];
                
                NSRect  rect = [NSWindow contentRectForFrameRect: [self frame] styleMask: [self styleMask]];
                
                int offsetY=0;
                
                if(trackingPinch)
                    offsetY=50;
                
                MouseEventArray &events=application->mouseEvents();                
                events.push_back(MouseInput(MouseState::Up,(int)(location.x),
                                            (int)(rect.size.height -(location.y))-offsetY,0,eventId,(unsigned long)([event timestamp]*1000.0)));                
                
                if(trackingPinch)
                {
                    int posX=(int)(location.x);
                    int posY=(int)(rect.size.height -(location.y))+offsetY;
                    
                    trackingPinch=false;
                    posX=-(posX-pinchPosX);
                    posX+=pinchPosX;
                    posY=-(posY-pinchPosY);
                    posY+=pinchPosY;
                    events.push_back(MouseInput(MouseState::Up,posX,
                                                posY,0,pinchEventId,(unsigned long)([event timestamp]*1000.0)));                      
                    
                }                
                
                
            }
                break;
                
            default: {}
        }
    }
    */
}

- (void)mouseDragged:(NSEvent*)event
{    
    /*
    if(application)
    {
        switch([event type])
        {
                using namespace MiniEngine2;	                
                
            case NSLeftMouseDragged:
            {                
                NSPoint location = [event locationInWindow];
                
                NSRect  rect = [NSWindow contentRectForFrameRect: [self frame] styleMask: [self styleMask]];
                
                int offsetY=0;
                
                if(trackingPinch)
                    offsetY=50;                
                
                MouseEventArray &events=application->mouseEvents();                
                events.push_back(MouseInput(MouseState::Moving,(int)(location.x),
                                            (int)(rect.size.height -(location.y))-offsetY,0, eventId,(unsigned long)([event timestamp]*1000.0)));  
                
                
                if(trackingPinch)
                {
                    int posX=(int)(location.x);
                    int posY=(int)(rect.size.height -(location.y))+offsetY;
                    
                    posX=-(posX-pinchPosX);
                    posX+=pinchPosX;
                    posY=-(posY-pinchPosY);
                    posY+=pinchPosY;
                    events.push_back(MouseInput(MouseState::Moving,posX,
                                                posY,0, pinchEventId,(unsigned long)([event timestamp]*1000.0)));                      
                    
                }
                
            }
                break;
                
            default: {}
        }
    }
     */
}

-(BOOL)canBecomeKeyWindow
{
    return YES;
}

@end


