//
// Created by Walter Rawdanik on 10/19/15.
//

#ifndef MINIENGINE_APPLICATION_H
#define MINIENGINE_APPLICATION_H

namespace MiniEngine
{
    class RenderManager;
    class FrameTime;
    class InternalApplication;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        virtual void onInit(RenderManager *manager);
        virtual void onUpdate(FrameTime &frameTime);
        virtual void onActivate();
        virtual void onExit();
        virtual void onSuspend();
        virtual void onResume();
        virtual void onOrientationChange();


        static void runApplication(Application *app,int argc=0, char *argv[]=0);

    private:

        void setInternalApplication(InternalApplication *app);

        InternalApplication *mInternalApp;

    };
}

#endif //MINIENGINE_APPLICATION_H
