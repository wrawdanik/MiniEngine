

#import <AppKit/NSApplication.h>
#import <AppKit/NSWindow.h>


namespace MiniEngine
{
    class   InternalApplication;
};

@interface MiniEngineApplicationDelegate: NSObject <NSApplicationDelegate>
{

}
- (void)_applicationLoop;
- (void)_aboutMenu:(id)sender;
- (void)_capsMenu:(id)sender;
- (void)_orientMenu:(id)sender;

@property (assign) MiniEngine::InternalApplication * application;

@end

@interface AppWindow : NSWindow < NSWindowDelegate>
{
@private
    
    MiniEngine::InternalApplication *application;
}


@property (assign) MiniEngine::InternalApplication * application;
@end

