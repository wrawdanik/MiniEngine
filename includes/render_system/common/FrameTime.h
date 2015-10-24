//
// Created by Walter Rawdanik on 10/19/15.
//

#ifndef MINIENGINE_FRAMETIME_H
#define MINIENGINE_FRAMETIME_H


namespace MiniEngine
{
    struct FrameTime
    {
        FrameTime()
        {
            currentTimeMillis = elapsedTimeMillis=frame=0;
            elapsedTimeSeconds=0;
        }

        uint64_t currentTimeMillis;
        float    elapsedTimeSeconds;
        uint64_t elapsedTimeMillis;
        uint64_t frame;
    };
}

#endif //MINIENGINE_FRAMETIME_H
