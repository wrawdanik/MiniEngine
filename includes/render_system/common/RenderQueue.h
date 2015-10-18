#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include "RenderSortMask.h"
#include "Color.h"
#include "Viewport.h"
#include "rdestl.h"
#include "DataTypes.h"
#include "Transform3d.h"
#include "RenderResourceId.h"

namespace MiniEngine
{

    struct RenderCommandClear
    {
        RenderCommandClear()
        {
            bits = static_cast<size_t>(ClearBits::Color) | static_cast<size_t>(ClearBits::Depth);
            depth = 0;
            stencil = 0;
            color = 0;
        }

        RenderCommandClear(size_t clearBits, const Color &cColor, float cDepth, int cStencil)
        {
            color = cColor.rgba();
            bits = clearBits;
            depth = cDepth;
            stencil = cStencil;
        }

        uint16_t bits;
        uint16_t stencil;
        uint32_t color;
        float depth;
    };

    struct RenderCommandCamera
    {
        RenderCommandCamera()
        {
            fov=aspectRatio=nearClip=farClip=0;
        }

        float		fov;
        float		aspectRatio;
        float		nearClip;
        float		farClip;
        Transform3d transform;
    };

    struct RenderCommandViewport
    {
        RectangleI viewport;
    };

    struct RenderCommandRenderableItem
    {
        Transform3d transform;
        RenderResourceId renderableId;
        RenderResourceId materialId;
    };
    using RenderCommandClearArray=rde::vector<RenderCommandClear>;
    using RenderCommandCameraArray=rde::vector<RenderCommandCamera>;
    using RenderCommandViewportArray=rde::vector<RenderCommandViewport>;
    using RenderCommandRenderableItemArray=rde::vector<RenderCommandRenderableItem>;



    struct RenderQueueData
    {
        RenderCommandClearArray clearArray;
        RenderCommandCameraArray cameraArray;
        RenderCommandViewportArray viewportArray;
        RenderResourceIdArray   resourceIdArray;
        RenderCommandRenderableItemArray renderableItemArray;
    };

    struct RenderQueueEntry
    {
        RenderQueueEntry()
        {

        }

        RenderQueueEntry(uint32_t itemIndex)
        {
            renderSortMask.renderableItem(itemIndex);
        }

        RenderQueueEntry(uint32_t itemIndex, RenderSortMask &mask)
        {
            renderSortMask = mask;
            renderSortMask.renderableItem(itemIndex);
        }

        inline bool operator<(const RenderQueueEntry &rhs) const
        {
            if (renderSortMask.values[4] != rhs.renderSortMask.values[4])
                return (renderSortMask.values[4] < rhs.renderSortMask.values[4]);
            if (renderSortMask.values[3] != rhs.renderSortMask.values[3])
                return (renderSortMask.values[3] < rhs.renderSortMask.values[3]);
            if (renderSortMask.values[2] != rhs.renderSortMask.values[2])
                return (renderSortMask.values[2] < rhs.renderSortMask.values[2]);
            if (renderSortMask.values[1] != rhs.renderSortMask.values[1])
                return (renderSortMask.values[1] < rhs.renderSortMask.values[1]);
            return (renderSortMask.values[0] < rhs.renderSortMask.values[0]);
        }

        inline bool operator>(const RenderQueueEntry &rhs) const
        {

            if (renderSortMask.values[4] != rhs.renderSortMask.values[4])
                return (renderSortMask.values[4] > rhs.renderSortMask.values[4]);
            if (renderSortMask.values[3] != rhs.renderSortMask.values[3])
                return (renderSortMask.values[3] > rhs.renderSortMask.values[3]);
            if (renderSortMask.values[2] != rhs.renderSortMask.values[2])
                return (renderSortMask.values[2] > rhs.renderSortMask.values[2]);
            if (renderSortMask.values[1] != rhs.renderSortMask.values[1])
                return (renderSortMask.values[1] > rhs.renderSortMask.values[1]);
            return (renderSortMask.values[0] > rhs.renderSortMask.values[0]);
        }

        inline bool operator==(const RenderQueueEntry &rhs) const
        {
            return (renderSortMask.values[4] == rhs.renderSortMask.values[4]) &&
                   (renderSortMask.values[3] == rhs.renderSortMask.values[3] &&
                    renderSortMask.values[2] == rhs.renderSortMask.values[2]) &&
                   (renderSortMask.values[1] == rhs.renderSortMask.values[1] &&
                    renderSortMask.values[0] == rhs.renderSortMask.values[0]);

        }

        RenderSortMask renderSortMask;
    };

/*

    typedef rde::vector<RenderQueueEntry> RenderQueueEntryArray;

    typedef rde::vector<rde::string> RenderQueueDebugArray;

    class RenderQueue
    {
    public:


        RenderQueue(RenderTarget *target);


        void submit(RenderableItemPtr item);

        inline void submit(const RenderableItemPtrArray &items)
        {
            if (!items.empty())
                submit(&items[0], items.size());
        }jmjjmmj

        void submit(const RenderableItemPtr *items, int count);

        void submitClearViewport(unsigned int framebufferBits, const Color &color, float depth = 0, int stencil = 0);

        void submitLightsOn(const LightPtrArray &lights);

        void submitLightsOff();


        inline RenderableItem *decodeItem(const RenderQueueEntry &entry)
        {
            return mRendItems[entry.renderSortMask.renderableItem()].get();
        }

        void setCurrentViewport(const Viewport &view);

        void setCurrentCamera(CameraPtr cam);

        inline CameraPtr currentCamera() const
        { return mCurrentCamera; }

        inline const Viewport &currentViewport() const
        { return mViewportArray.back(); }


        void setDefaultCamera(CameraPtr camera);

        inline void setDefaultClearCommand(const RenderCommandClear &command)
        { mDefaultClearCommand = command; }

        inline const RenderCommandClear &defaultClearCommand() const
        { return mDefaultClearCommand; }

        inline CameraPtr defaultCamera() const
        { return mDefaultCamera; }

        inline const CameraPtrArray &cameras()
        { return mCameraArray; }

        inline const ViewportArray &viewports()
        { return mViewportArray; }

        void clear();

        void prepareForRendering();

        void finishRendering();

        typedef rde::vector<RenderCommandClear> RenderCommandClearArray;


        struct LightCommands
        {
            LightPtrArray lightArray;
            rde::vector<int> lightIndexArray;
        };

        inline const RenderQueueEntryArray &compiledEntries() const
        { return mEntries; }

        inline RenderCommandClearArray &compiledClearCommands()
        { return mCommandsClear; }

        inline LightCommands &compiledLightCommands()
        { return mCommandsLights; }

        inline const CameraPtrArray &compiledCameraCommands() const
        { return mCameraArray; }

        inline const ViewportArray &compiledViewportCommands() const
        { return mViewportArray; }

        inline bool hasDirtyMaterials() const
        { return mDirtyMaterialsCount > 0; }

    private:

        void submitEmptyItem();

        void clearInternal();

        void updateItems();

        RenderCommandClearArray mCommandsClear;
        LightCommands mCommandsLights;
        RenderQueueEntryArray mEntries;
        CameraPtrArray mCameraArray;
        ViewportArray mViewportArray;


        uint32_t mDirtyMaterialsCount;
        int mCurrentLightIndex;
        int mLightIndexToEnable;
        int mCurrentClearCommandEntry;
        RenderableItemPtrArray mRendItems;
        CameraPtr mCurrentCamera;
        CameraPtr mDefaultCamera;
        int mCurrentViewportIndex;
        int mCurrentCameraIndex;
        RenderCommandClear mDefaultClearCommand;
        RenderQueueDebugArray mDebugArray;

        RenderTarget *mMyTarget;


        bool mQueueCleared;
    };
*/
}

#endif
