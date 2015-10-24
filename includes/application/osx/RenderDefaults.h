//
// Created by Walter Rawdanik on 10/23/15.
//

#ifndef MINIENGINE_PLATFORMRENDERDEFAULTS_H
#define MINIENGINE_PLATFORMRENDERDEFAULTS_H

namespace MiniEngine
{
    class RenderDefaults
    {
    public:

        static constexpr size_t vertexAlignment()
        {
            return 4;
        }
        static constexpr size_t backendBufferInstances()
        {
            return 3;
        }

    };
}



#endif //MINIENGINE_PLATFORMRENDERDEFAULTS_H
