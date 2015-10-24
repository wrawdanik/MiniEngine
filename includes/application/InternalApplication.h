//
// Created by Walter Rawdanik on 10/20/15.
//

#ifndef MINIENGINE_INTERNALAPPLICATION_H
#define MINIENGINE_INTERNALAPPLICATION_H

#include "RenderTarget.h"


namespace MiniEngine
{
    class Application;

    class InternalApplication
    {
    public:

        InternalApplication(Application *app);
        virtual ~InternalApplication();
        void runApplication(int args, char *argv[]);
        void destroyApplication();
        void frameStarted();
        void frameEnded();

    protected:

        void run();

        RenderTargetPtr createDefaultTarget();


    private:
        Application *mApplication;


    };
}


#endif //MINIENGINE_INTERNALAPPLICATION_H
