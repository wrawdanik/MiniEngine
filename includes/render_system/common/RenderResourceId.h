
#ifndef RENDER_RESOURCE_ID
#define RENDER_RESOURCE_ID

#include "DataTypes.h"
#include "rdestl.h"
#include "Status.h"
#include "Task.h"

namespace MiniEngine
{


    class RenderResourceId
    {
    public:

        RenderResourceId()
        {
            mId.value=0;

        }

        RenderResourceId(uint64_t val)
        {
            mId.value=val;
        }

        RenderResourceId(size_t frame,size_t uid, ResourceType type,RenderBufferMode mode, RenderBufferSource  source=RenderBufferSource::Server)
        {
            mId.value=0;
            mId.uid=uid;
            mId.frame=frame;
            mId.valid=1;
            mId.type= static_cast<size_t>(type);
            mId.source=static_cast<size_t>(source);
            mId.mode=static_cast<size_t>(mode);
        }


        inline bool isValid() const
        {
            return (mId.valid!=0);
        }

        inline operator uint64_t () const
        {
            return mId.value;
        }

        inline ResourceType type() const
        {
            return static_cast<ResourceType>(mId.type);
        }

        inline size_t frame() const
        {
            return mId.frame;
        }

        inline size_t uid() const
        {
            return mId.uid;
        }

        inline bool isMapped() const
        {
            return (mId.mapped!=0);
        }

        inline RenderBufferSource source() const
        {
            return static_cast<RenderBufferSource >(mId.source);
        }

        inline RenderBufferMode mode() const
        {
            return static_cast<RenderBufferMode>(mId.mode);
        }



        inline void update(bool valid, bool mapped)
        {
            mId.valid=valid;
            mId.mapped=mapped;
        }

        inline bool operator == ( const RenderResourceId& rhs ) const
        {
            return (mId.uid==rhs.mId.uid);
        }


    private:

        union IdData
        {
            struct
            {
                size_t     valid:   1;
                size_t     mapped:  1;
                size_t     type:    4;          // ResourceType::
                size_t     mode:    1;          // RenderBufferMode::Static, RenderBufferMode::Dynamic
                size_t     source:    1;    // client side or server side
                size_t     frame:   28;
                size_t     uid:     28;
            };


            uint64_t value;
        };

        IdData  mId;
    };

    using  RenderResourceIdArray=rde::vector<RenderResourceId>;
    using  RenderResourceIdMap=rde::hash_map<size_t,RenderResourceId>;

    struct RenderResourceIdBuffer
    {
        RenderResourceIdBuffer()
        {
            buffer = nullptr;
            sizeInBytes = 0;
        }


        RenderResourceIdBuffer(RenderResourceId id)
        {
            this->id = id;
            buffer = nullptr;
            sizeInBytes = 0;
        }

        RenderResourceIdBuffer(RenderResourceId id, unsigned char *buffer, size_t sizeInBytes)
        {
            this->id = id;
            this->buffer = buffer;
            this->sizeInBytes = sizeInBytes;

        }

        inline bool isValid() const
        {
            return id.isMapped();
        }

        RenderResourceId id;
        TaskId lockingTaskId;
        unsigned char *buffer;
        size_t sizeInBytes;
    };

}
#endif


