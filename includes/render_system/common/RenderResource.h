#ifndef RESOURCE_H
#define RESOURCE_H

#include "IntrusiveTracked.h"
#include "RenderResourceId.h"
#include "RenderManagerProxy.h"
#include "Task.h"

namespace MiniEngine
{
	class Resource;
	class RenderManager;


	class Resource : public IntrusiveTracked
	{
	public:
		Resource(RenderManager *manager) ;
		virtual ~Resource()=default;

		Resource &	operator=(const Resource & that)=delete;
		Resource(const Resource &)=delete;

		inline RenderManager* manager()  { return mManagerProxy.manager();}
		inline bool isValid() const { return mValid;}

		StatusPtr status() const;

	protected:

		void setStatus(StatusPtr e) { mStatus=e;}

		inline RenderManagerProxy& handler()  { return  mManagerProxy;}

		inline void setValid(bool value) { mValid=value;}

	private:
		StatusPtr mStatus;
		bool   mValid;
		RenderManagerProxy mManagerProxy;
	};


	class RenderResource: public Resource
	{
	public:	

		RenderResource(RenderManager *manager) ;
		virtual ~RenderResource();

		RenderResourceIdBuffer memoryBuffer(RenderBufferSource source=RenderBufferSource::Server);

		inline RenderResourceId id(RenderBufferSource source=RenderBufferSource::Server) const { return mId[static_cast<size_t>(source)];}
		inline void setId(RenderResourceId id,RenderBufferSource source=RenderBufferSource::Server )
		{
			mId[static_cast<size_t>(source)]=id;
		}







	private:


        RenderResourceId mId[2];


	};

    

}



#endif
