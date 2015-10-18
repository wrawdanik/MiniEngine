//
// Created by Walter Rawdanik on 10/15/15.
//

#include "RenderManagerProxy.h"
#include "RenderManager.h"

using namespace MiniEngine;

Status::Code RenderManagerProxy::renderTaskStatusCode(TaskId taskId, bool discard)
{
    //return mManager->renderTaskCode(taskId,discard);
    return Status::Code::InvalidState;
}

void RenderManagerProxy::releaseResource(RenderResourceId id)
{
    mManager->releaseResource(id);
}