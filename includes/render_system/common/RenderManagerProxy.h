//
// Created by Walter Rawdanik on 10/15/15.
//

#ifndef MINIENGINE_RENDERMANAGERPROXY_H
#define MINIENGINE_RENDERMANAGERPROXY_H

#include "RenderResourceId.h"
#include "Task.h"

namespace MiniEngine
{

    class RenderManager;

    class RenderManagerProxy
    {
    public:
        RenderManagerProxy(RenderManager *manager)
        {
            this->mManager = manager;
        }

        void releaseResource(RenderResourceId id);

        Status::Code renderTaskStatusCode(TaskId taskId, bool discard=false);

        RenderManager *manager() const
        { return mManager; }


    private:

        RenderManager *mManager;
    };

}

#endif //MINIENGINE_RENDERMANAGERPROXY_H
