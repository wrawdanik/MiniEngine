
#ifndef RENDER_SORT_MASK_H
#define RENDER_SORT_MASK_H

#include "stdint.h"

namespace MiniEngine
{

	union RenderSortMask
	{
/*		
		Use manual implementation of the following bitfields ( compiler generated bitfields are not portable enough for this purpose (sorting) )

		GLES

		struct
		{
            // 192
 
            uint32_t viewport                   :10   
            uint32_t globalGroup                :10 
            uint32_t camera                     :10      
            uint32_t blend                      :1    
            uint32_t alphaTest                  :1
 
            uint32_t depthWrite                 :1
            uint32_t depthTest                  :1            
            uint32_t localGroup                 :10      
            uint32_t renderableItemIndex        :16
 
            uint32_t program                    :16    
            uint32_t texture1                   :16    
 
            uint32_t texture2                   :16    
            uint32_t renderable                 :16    
 
            uint32_t lightIndex                 :12 
            uint32_t fogIndex                   :10  
            uint32_t clearIndex                 :10   
 

		};


*/


        
        enum 
        {
            ViewportShift=22,
            CameraShift=2,
            GlobalGroupShift=12,
            BlendShift=1,
            AlphaTestShift=0,            
            
            DepthWriteShift=27,
            DepthTestShift=26,
            LocalGroupShift=16,
            RenderableItemIndexShift=0,  
            
            ProgramShift=16,
            Texture1Shift=0,
            
            Texture2Shift=16,
            RenderableShift=0,
            
            
            LightIndexShift=20,
            FogIndexShift=10,
            ClearIndexShift=10            
            
        };

        enum 
        {
            ViewportMask=0x3FF,
            CameraMask=0x3FF,
            GlobalGroupMask=0x3FF,
            BlendMask=0x1,
            AlphaTestMask=0x1,
            
            DepthWriteMask=0x1,
            DepthTestMask=0x1, 
            LocalGroupMask=0x3FF,
            RenderableItemIndexMask=0xFFFF,
            
            ProgramMask=0xFFFF,            
            Texture1Mask=0xFFFF,
            
            Texture2Mask=0xFFFF,
            RenderableMask=0xFFFF,
            
            
            LightIndexMask=0xFFF,
            FogIndexMask=0x3FF,     
            ClearIndexMask=0x3FF                 
            
            
        };       
        
        enum 
        {
            ViewportInvMask=~(ViewportMask<<ViewportShift),
            CameraInvMask=~(CameraMask<<CameraShift),
            GlobalGroupInvMask=~(GlobalGroupMask<<GlobalGroupShift),
            BlendInvMask=~(BlendMask<<BlendShift),
            AlphaTestInvMask=~(AlphaTestMask<<AlphaTestShift),
            
            DepthWriteInvMask=~(DepthWriteMask<<DepthWriteShift),
            DepthTestInvMask=~(DepthTestMask<<DepthTestShift),
            LocalGroupInvMask=~(LocalGroupMask<<LocalGroupShift),
            RenderableItemIndexInvMask=~(RenderableItemIndexMask<<RenderableItemIndexShift),
            
            ProgramInvMask=~(ProgramMask<<ProgramShift),
            Texture1InvMask=~(Texture1Mask<<Texture1Shift),
            
            Texture2InvMask=~(Texture2Mask<<Texture2Shift),
            RenderableInvMask=~(RenderableMask<<RenderableShift),
            
            LightIndexInvMask=~(LightIndexMask<<LightIndexShift),
            FogIndexInvMask=~(FogIndexMask<<FogIndexShift),
            ClearIndexInvMask=~(ClearIndexMask<<ClearIndexShift),
        };         
        
        enum 
        {
            ViewportPos=4,
            CameraPos=4,
            GlobalGroupPos=4,
            BlendPos=4,
            AlphaTestPos=4,
            
            DepthWritePos=3,
            DepthTestPos=3,
            LocalGroupPos=3,
            RenderableItemIndexPos=3,
            
            ProgramPos=2,
            Texture1Pos=2,
            
            Texture2Pos=1,
            RenderablePos=1,  
            
            LightIndexPos=0,
            FogIndexPos=0,
            ClearIndexPos=0

        };
        
		enum 
		{
			MaxViewport=(1<<10)-1,
			MaxCamera=(1<<10)-1,
			MaxGlobalGroup=(1<<10)-1,
			MaxLocalGroup=(1<<10)-1,

			MaxLightIndex=(1<<12)-1,
			MaxFogIndex=(1<<10)-1,
			MaxClearIndex=(1<<10)-1,
            MaxRenderableItemIndex=(1<<16)-1
		};

        
        
		RenderSortMask()
		{
            reset();
                                    
		}

		void reset()
		{
			values[0]=0;
			values[1]=0;
			values[2]=0;
			values[3]=0;
			values[4]=0;
		}

		inline uint32_t viewport() const { return (uint32_t)((values[ViewportPos]>>ViewportShift) & ViewportMask);}
		inline void viewport(uint32_t id) { values[ViewportPos]=((values[ViewportPos]& ViewportInvMask) | (((uint32_t)(id))<<ViewportShift));}

		inline uint32_t camera() const { return (uint32_t)((values[CameraPos]>>CameraShift) & CameraMask);}
		inline void camera(uint32_t id) { values[CameraPos]=((values[CameraPos]& CameraInvMask) | (((uint32_t)(id))<<CameraShift));}


		static inline uint32_t clampGlobalGroup(uint32_t group)
		{
			if(group>MaxGlobalGroup)
				return MaxGlobalGroup;
			return group;
		}

		static inline uint32_t clampLocalGroup(uint32_t group)
		{
			if(group>MaxGlobalGroup)
				return MaxLocalGroup;
			return group;
		}
        
		inline uint32_t renderableItem() const { return (uint32_t)((values[RenderableItemIndexPos]>>RenderableItemIndexShift) & RenderableItemIndexMask);}
		inline void renderableItem(uint32_t id) { values[RenderableItemIndexPos]=((values[RenderableItemIndexPos]& RenderableItemIndexInvMask) | ((uint32_t)id<<RenderableItemIndexShift));}        

		inline uint32_t globalGroup() const { return (uint32_t)((values[GlobalGroupPos]>>GlobalGroupShift) & GlobalGroupMask);}
		inline void globalGroup(uint32_t id) { values[GlobalGroupPos]=((values[GlobalGroupPos]& GlobalGroupInvMask) | ((uint32_t)id<<GlobalGroupShift));}

		inline uint32_t blend() const { return (uint32_t)((values[BlendPos]>>BlendShift)& BlendMask);}
		inline void blend(uint32_t id) { values[BlendPos]=((values[BlendPos]& BlendInvMask) | ((uint32_t)id<<BlendShift));}

		inline uint32_t alphaTest() const { return (uint32_t)((values[AlphaTestPos]>>AlphaTestShift)& AlphaTestMask);}
		inline void alphaTest(uint32_t id) { values[AlphaTestPos]=((values[AlphaTestPos]& AlphaTestInvMask) | ((uint32_t)id<<AlphaTestShift));}

		inline uint32_t depthWrite() const { return (uint32_t)((values[DepthWritePos]>>DepthWriteShift) & DepthWriteMask);}
		inline void depthWrite(uint32_t id) { values[DepthWritePos]=((values[DepthWritePos]& DepthWriteInvMask) | ((uint32_t)id<<DepthWriteShift));}

		inline uint32_t depthTest() const { return (uint32_t)((values[DepthTestPos]>>DepthTestShift) & DepthTestMask);}
		inline void depthTest(uint32_t id) { values[DepthTestPos]=((values[DepthTestPos]& DepthTestInvMask) | ((uint32_t)id<<DepthTestShift));}
        
		inline uint32_t program() const { return (uint32_t)((values[ProgramPos]>>ProgramShift) & ProgramMask);}
		inline void program(uint32_t id) { values[ProgramPos]=((values[ProgramPos]& ProgramInvMask) | ((uint32_t)id<<ProgramShift));}

		inline uint32_t texture1() const { return (uint32_t)((values[Texture1Pos]>>Texture1Shift) & Texture1Mask);}
		inline void texture1(uint32_t id) { values[Texture1Pos]=((values[Texture1Pos]& Texture1InvMask) | ((uint32_t)id<<Texture1Shift));}        

		inline uint32_t texture2() const { return (uint32_t)((values[Texture2Pos]>>Texture2Shift) & Texture2Mask);}
		inline void texture2(uint32_t id) { values[Texture2Pos]=((values[Texture2Pos]& Texture2InvMask) | ((uint32_t)id<<Texture2Shift));}         

		inline uint32_t renderable() const { return (uint32_t)((values[RenderablePos]>>RenderableShift) & RenderableMask);}
		inline void renderable(uint32_t id) { values[RenderablePos]=((values[RenderablePos]& RenderableInvMask) | ((uint32_t)id<<RenderableShift));}                 
        
        
		inline int localGroup() const { return (uint32_t)((values[LocalGroupPos]>>LocalGroupShift) & LocalGroupMask);}
		inline void localGroup(uint32_t id) { values[LocalGroupPos]=((values[LocalGroupPos]& LocalGroupInvMask) | ((uint32_t)id<<LocalGroupShift));}


		inline int lightIndex() const { return (uint32_t)((values[LightIndexPos]>>LightIndexShift) & LightIndexMask);}
		inline void lightIndex(uint32_t id) { values[LightIndexPos]=((values[LightIndexPos]& LightIndexInvMask) | ((uint32_t)id<<LightIndexShift));}


		inline int fogIndex() const { return (uint32_t)((values[FogIndexPos]>>FogIndexShift) & FogIndexMask);}
		inline void fogIndex(uint32_t id) { values[FogIndexPos]=((values[FogIndexPos]& FogIndexInvMask) | ((uint32_t)id<<FogIndexShift));}

		inline int clearIndex() const { return (uint32_t)(values[ClearIndexPos] & ClearIndexMask);}
		inline void clearIndex(uint32_t id) { values[ClearIndexPos]=(values[ClearIndexPos]& ClearIndexInvMask) | ((uint32_t)id);}

		uint32_t values[5];

        
        
        
	};
}

#endif
