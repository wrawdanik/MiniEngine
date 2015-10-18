/*
-----------------------------------------------------------------------------

 Copyright (c) 2009 Walter Rawdanik	

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "RenderQueue.h"
#include "RenderSortMask.h"
#include "Log.h"

using namespace MiniEngine;


/*
RenderQueue::RenderQueue(RenderTarget *target)
{
    mMyTarget=target;
    mRendItems.reserve(128);
	mCommandsLights.lightArray.reserve(64);
	mCommandsLights.lightIndexArray.reserve(64);
	mEntries.reserve(64);
	//mResManager=Application::instance()->resourceManager();
	clear();
    mQueueCleared=true;
    _Log("mDefault camera value is %d",mDefaultCamera.get());
}

void RenderQueue::updateItems()
{	
    
	    
    
}


void RenderQueue::submit(RenderableItemPtr item) 
{    
    
    unsigned int maxw=RenderSortMask::MaxRenderableItemIndex;
    
    
    if(!mQueueCleared)
    {
        clear();
    }
     
	RenderSortMask mask;    
    RenderableItem *itemPtr=item.get();
#ifdef _DEBUG
    
    if(mRendItems.size()==RenderSortMask::MaxRenderableItemIndex)
    {
        _LogDebug("RenderQueue::submit : Command overflow. Maximum of %d renderable item instances allowed per frame.",RenderSortMask::MaxRenderableItemIndex);
        return;
    }
    if(itemPtr==0)
    {
        _LogDebug("RenderQueue::submit  : Invalid (null) RenderableItemPtr submitted.");
        return;
    }
    if(itemPtr->renderablePtr()==0)
    {
        _LogDebug("RenderQueue::submit  : Invalid (null) RenderablePtr submitted for item %d", itemPtr->resourceId());
        return;           
    }
    
#endif            
    
    Material *mat=item->materialPtr();
#ifdef _DEBUG        
    
    if(mat==0)
    {
        _LogDebug("RenderQueue::submit  : Invalid (null) MaterialPtr submitted for item %d", item->resourceId());
        return;
    }
#endif   
    mat->updateRenderSortMask(mask);     
    
    mask.renderable(itemPtr->renderablePtr()->resourceId());    
    
    mask.globalGroup(item->renderGroupGlobal());
    mask.localGroup(item->renderGroupLocal());       
	mask.viewport(mCurrentViewportIndex);
	mask.camera(mCurrentCameraIndex);
	mask.lightIndex(mLightIndexToEnable);
	mask.clearIndex(mCurrentClearCommandEntry);
    mRendItems.push_back(item);    
    uint32_t itemCount=((uint32_t)mRendItems.size())-1;
    mask.renderableItem(itemCount);
	mEntries.push_back(RenderQueueEntry(itemCount,mask));
#ifdef _DEBUG
	if(mask.viewport()==RenderSortMask::MaxViewport)
		_LogDebug("RenderQueue::submit  : No valid viewport defined. Call RenderQueue::setCurrentViewport.");
	if(mask.camera()==RenderSortMask::MaxCamera)
		_LogDebug("RenderQueue::submit  : No valid camera defined. Call RenderQueue::setCurrentCamera or RenderTarget::setDefaultCamera");
#endif	
	
}

void RenderQueue::submit(const RenderableItemPtr *items, int count)
{
    if(!mQueueCleared)
    {
        clear();
    }    
    
	const RenderableItemPtr *current=&items[0];    
    
	while(count)
	{	
		RenderSortMask mask;        
        RenderableItem *item=current->get();
        
#ifdef _DEBUG                  
        if((uint32_t)mRendItems.size()==RenderSortMask::MaxRenderableItemIndex)
        {
            _LogDebug("RenderQueue::submit : Command overflow. Maximum of %d renderable item instances allowed per frame.",RenderSortMask::MaxRenderableItemIndex);
            return;
        }        
        
        if(item==0)
        {
            _LogDebug("RenderQueue::submit  : Invalid (null) RenderableItemPtr submitted.");
            ++current;
            --count;
            continue;
        }
        if(item->renderablePtr()==0)
        {
            _LogDebug("RenderQueue::submit  : Invalid (null) RenderablePtr submitted for item %d", item->resourceId());
            ++current;
            --count;
            continue;            
        }
        
#endif        
        mask.renderable(item->renderablePtr()->resourceId());    
        Material *mat=item->materialPtr();
#ifdef _DEBUG        
            
        if(mat==0)
        {
            _LogDebug("RenderQueue::submit  : Invalid (null) MaterialPtr submitted for item %d", item->resourceId());
            ++current;
            --count;
            continue; 
        }
#endif   
        mat->updateRenderSortMask(mask);        
        Texture *tex1=mat->textureUnit(0).texture().get();
        if(tex1)        
            mask.texture1(tex1->resourceId());
        Texture *tex2=mat->textureUnit(1).texture().get();
        if(tex2)        
            mask.texture2(tex2->resourceId());               
        
        mask.globalGroup(item->renderGroupGlobal());
        mask.localGroup(item->renderGroupLocal());        
		mask.viewport(mCurrentViewportIndex);
		mask.camera(mCurrentCameraIndex);
		mask.lightIndex(mLightIndexToEnable);
		mask.clearIndex(mCurrentClearCommandEntry);
        mRendItems.push_back(*current);    
        uint32_t itemCount=((uint32_t)mRendItems.size())-1;
        mask.renderableItem(itemCount);        
        mEntries.push_back(RenderQueueEntry(itemCount,mask));        
#ifdef _DEBUG
		if(mask.viewport()==RenderSortMask::MaxViewport)
			_LogDebug("RenderQueue::submit  : No valid viewport defined. Call RenderQueue::setCurrentViewport or RenderTarget::setDefaultViewport");
		if(mask.camera()==RenderSortMask::MaxCamera)
			_LogDebug("RenderQueue::submit  : No valid camera defined. Call RenderQueue::setCurrentCamera or RenderTarget::setDefaultCamera");
#endif	
		++current;
		--count;
	}	
}

void RenderQueue::submitLightsOn(const LightPtrArray  &lights)
{
	if(!lights.empty())
	{        
#ifdef _DEBUG        
		if(mCommandsLights.lightIndexArray.size()==RenderSortMask::MaxLightIndex)
		{
			_LogDebug("RenderQueue::submitLightsOnCommand : Command overflow. Maximum of %d light instances allowed per frame.",RenderSortMask::MaxLightIndex);
			return;
		}
#endif
        if(!mQueueCleared)
        {
            clear();
        }        
		++mCurrentLightIndex;
		mCommandsLights.lightIndexArray.push_back(mCommandsLights.lightIndexArray.size());
		
		LightPtrArray::const_iterator current=lights.begin();
		LightPtrArray::const_iterator last=lights.end();
		while(current!=last)
		{
			mCommandsLights.lightArray.push_back(*current);
			++current;
		}
		mLightIndexToEnable=mCurrentLightIndex;
	}	
}
void RenderQueue::submitLightsOff()
{
    if(!mQueueCleared)
    {
        clear();
    }      
	mLightIndexToEnable=-1;	  
}

void RenderQueue::submitClearViewport(unsigned int framebufferBits,const Color &color, float depth, int stencil)
{
#ifdef _DEBUG    
	if(mCommandsClear.size()==RenderSortMask::MaxClearIndex)
	{
		_LogDebug("RenderQueue::submitClearCommand : Command overflow. Maximum of %d submitClearViewport commands allowed.",RenderSortMask::MaxClearIndex);
		return;
	}
#endif    
    if(!mQueueCleared)
    {
        clear();
    }        
	mCommandsClear.push_back(RenderCommandClear(framebufferBits,color,depth,stencil));
	mCurrentClearCommandEntry=mCommandsClear.size()-1;
}
void RenderQueue::clear()
{
    mQueueCleared=true;
	mCurrentLightIndex=-1;
	mCurrentCameraIndex=-1;
	mCurrentViewportIndex=-1;
	mLightIndexToEnable=-1;
	mCommandsLights.lightArray.clear_fast();
	mCommandsLights.lightIndexArray.clear_fast();
	mEntries.clear_fast();
	mCommandsClear.clear_fast();
	mCameraArray.clear_fast();
	mViewportArray.clear_fast();
    setCurrentViewport(mMyTarget->viewport()); 
	mCurrentCamera=0;
	if(mDefaultCamera.get()!=0)
	{
		setCurrentCamera(mDefaultCamera);
	}
    
    
	mCurrentClearCommandEntry=RenderSortMask::MaxClearIndex;
    mRendItems.clear();    
}

void RenderQueue::clearInternal()
{

}


void RenderQueue::prepareForRendering()
{

    
    RenderQueueEntryArray::iterator current=mEntries.begin();
    while(current!=mEntries.end())
    {
        RenderQueueEntry &cEntry=*current++;
        const uint32_t rendItemIndex=cEntry.renderSortMask.renderableItem();
        RenderableItem *item=mRendItems.at(rendItemIndex).get();        
        if(item->mMaterial->isSortMaskDirty())
        {
            ++mDirtyMaterialsCount;
            item->mMaterial->updateRenderSortMask(cEntry.renderSortMask);
        }                
    }
    
    if(mQueueCleared || mDirtyMaterialsCount)
    {
        if(mEntries.size()<20)
            rde::insertion_sort(mEntries.begin(),mEntries.end());
        else
            rde::quick_sort(mEntries.begin(),mEntries.end());
    }
}

void RenderQueue::finishRendering()
{
    mDirtyMaterialsCount=0;
    mQueueCleared=false;    
}

void RenderQueue::setDefaultCamera(CameraPtr cam) 
{
	mDefaultCamera=cam;
	if(mCameraArray.empty())
			setCurrentCamera(mDefaultCamera);
}

void RenderQueue::setCurrentViewport(const Viewport &view) 
{
    if(!mViewportArray.empty() && mViewportArray.back()==view)
		return;
    
#ifdef _DEBUG    
	if(mViewportArray.size()==RenderSortMask::MaxViewport)
	{
		_LogDebug("RenderQueue::setCurrentViewport : Command overflow. Maximum of %d setCurrentViewport commands allowed.",RenderSortMask::MaxViewport);
		return;
	}
#endif    
    if(!mQueueCleared)
        clear();
	mViewportArray.push_back(view);
	++mCurrentViewportIndex;
    
}
void RenderQueue::setCurrentCamera(CameraPtr cam)
{	
	if(mCurrentCamera==cam)
		return;
         
#ifdef _DEBUG        
	if(mCameraArray.size()==RenderSortMask::MaxCamera)
	{
		_LogDebug("RenderQueue::setCurrentCamera : Command overflow. Maximum of %d setCurrentCamera commands allowed.",RenderSortMask::MaxCamera);
		return;
	}
#endif    
    if(!mQueueCleared)
    {
        clear();
    }    
    
	mCurrentCamera=cam;	
	mCameraArray.push_back(mCurrentCamera);
	++mCurrentCameraIndex;

}

*/
