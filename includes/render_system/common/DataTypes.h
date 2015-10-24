#ifndef DATA_TYPES_H
#define DATA_TYPES_H


#include "Range.h"
#include <limits>
#include <stdint.h>

namespace MiniEngine
{

    enum class VertexBinding : size_t
    {
        Position,
        Color,
        Normal,
        UV,
        Tangent,
        Binormal,
        BoneWeight,
        BoneMatrix,
        Invalid
    };

    enum class ClearBits : size_t
    {
        Color=1,
        Depth=2,
        Stencil=4
    };

    enum class RenderData : size_t
    {
        Undefined,
        Float,
        HalfFloat,
        Int,
        UInt,
        UShort,
        Short,
        Byte,
        UByte,
        Bool
    };
    enum class RenderPrimitive : size_t
    {
        Point,
        Line,
        LineStrip,
        Triangle,
        TriangleStrip
    };

    enum class RenderCull : size_t
    {
        None,
        Front,
        Back
    };

    enum class RenderTriangleFill : size_t
    {
        Solid,
        Lines
    };

    struct DataTypes
    {
        static size_t sizeInBytes(RenderData type)
        {
            switch(type)
            {
                case RenderData::Float:
                case RenderData::Int:
                case RenderData::UInt:
                    return sizeof(float);
                case RenderData::UShort:
                case RenderData::Short:
                    return sizeof(short);
                case RenderData::Byte:
                case RenderData::UByte:
                    return sizeof(char);
                case RenderData::Bool:
                    return sizeof(bool);
                case RenderData::HalfFloat:
                    return sizeof(uint16_t);
                default:
                    return 0;
            }
        }
    };



    enum class  RenderFaceWinding: size_t
    {

            Clockwise,
            CounterClockwise
    };

    enum class  RenderBlendOperation: size_t
    {
            Add,
            Subtract,
            ReverseSubtract,
            Min,
            Max
    };

    enum class  RenderBlendFactor:  size_t
    {
            Zero,
            One,
            SourceColor,
            OneMinusSourceColor,
            SourceAlpha,
            OneMinusSourceAlpha,
            DestinationColor,
            OneMinusDestinationColor,
            DestinationAlpha,
            OneMinusDestinationAlpha,
            SourceAlphaSaturated,
            BlendColor,
            OneMinusBlendColor,
            BlendAlpha,
            OneMinusBlendAlpha
    };

    enum class  RenderBlendMode: size_t
    {
            Add,
            Modulate,
            Modulate2,
            ColorBlend,
            AlphaBlend,
            AlphaBlendPre,
            Copy,
            Custom
    };

    enum class  RenderBlendSource: size_t
    {
            ColorSource,
            ColorDestination,
            AlphaSource,
            AlphaDestination
    };

    enum class  RenderColorWriteMask: size_t
    {
            None,
            Red=0x1 << 1,
            Green=0x1 << 2,
            Blue=0x1 << 3,
            Alpha=0x1 << 4,
            All=0xf
    };

    enum class  RenderBufferMode: size_t
    {
        Static,
        Dynamic
    };

    enum class  RenderBufferSource: size_t
    {
        Server,
        Client
    };

    enum class  ResourceType: size_t
    {
        MemoryBuffer,
        Material,
        Texture,
        Camera,
        Light,
        RenderableItem,
        GPUProgramData,
    };


    enum class  CompressedPixelFormat: size_t
    {
            None,
            PVRTCI_2bpp_RGB,
            PVRTCI_2bpp_RGBA,
            PVRTCI_4bpp_RGB,
            PVRTCI_4bpp_RGBA,
            PVRTCII_2bpp,
            PVRTCII_4bpp,
            ETC1,
            DXT1,
            DXT2,
            DXT3,
            DXT4,
            DXT5,
            UYVY,
            YUY2,
            BW1bpp,
            SharedExponentR9G9B9E5,
            RGBG8888,
            GRGB8888,
            ETC2_RGB,
            ETC2_RGBA,
            ETC2_RGB_A1,
            EAC_R11,
            EAC_RG11
    };
    
    /*
     
     
        PixelFormat("rgba","8");
     
     */
    
    struct PixelFormat
    {
        PixelFormat():value(0)
        {
            
        }
        
        PixelFormat(const char *channels,const char *bits):value(0)
        {
            size_t shift=0;
            
            while(channels && shift<32)
            {
                value&=(uint64_t)*channels<<shift;
                shift+=8;
                channels++;
            }

            shift=0;
            
            while(bits && shift<32)
            {
                value&=(uint64_t)*bits<<(shift+32);
                shift+=8;
                bits++;
            }
            
        }
        
        inline bool isValid() const
        {
            return ((value!=0) && ((value>>32)!=0) && ((value&0xFFFFFFFF)!=0));
        }
        
        inline void format(char  *channels,char *bits) const
        {

            channels[0]=(char)value&0xFF;
            channels[1]=(char)value&0xFF00;
            channels[2]=(char)value&0xFF0000;
            channels[3]=(char)value&0xFF000000;
            
            bits[0]=(char)((value>>32)&0xFF);
            bits[1]=(char)((value>>32)&0xFF00);
            bits[2]=(char)((value>>32)&0xFF0000);
            bits[3]=(char)((value>>32)&0xFF000000);
        }


        static PixelFormat BGRA_8888()
        {
            return PixelFormat("BGRA","8888");
        }

    private:


        uint64_t value;
        

    };
    
/*
    struct PixelFormat
    {
        typedef enum
        {
            RGBA_4444,
            RGBA_5551,
            RGBA_8888,
            RGB_565,
            RGB_555,
            RGB_888,
            I_8,
            AI_88,
            BGRA_8888,
            A_8
        }Type;
    };
*/


    enum class  PixelColorSpace: size_t
    {
            RGB,
            sRGB
    };


    using    BufferItemRange=Range<size_t>;
    
}

#endif