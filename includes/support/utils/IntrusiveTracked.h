#ifndef INTRUSIVE_TRACKED_H
#define INTRUSIVE_TRACKED_H

#include "intrusive_ptr.h"

namespace MiniEngine
{


	class IntrusiveTracked
	{
	public:

		class Creator
		{
		public:

			virtual void release(IntrusiveTracked *object)=0;
		};

		IntrusiveTracked() : mReference(0),mCreator(nullptr)
		{

		}

		virtual ~IntrusiveTracked()
		{

		}

		inline size_t referenceCount() const
		{ return mReference; }


		inline void setCreator(Creator *creator) { mCreator=creator;}
		inline Creator*	creator() const { return mCreator;}

		inline void releaseObject(IntrusiveTracked *tracked)
		{
			if(mCreator && tracked!= nullptr)
			{
				mCreator->release(tracked);
				return;
			}
			delete tracked;
		}

	protected:



		friend void intrusive_ptr_add_ref(MiniEngine::IntrusiveTracked *p);

		friend void intrusive_ptr_release(MiniEngine::IntrusiveTracked *p);

		size_t mReference;
		Creator *mCreator;

	};

	inline void intrusive_ptr_add_ref(MiniEngine::IntrusiveTracked *p)
	{
		++(p->mReference);
	}

	inline void intrusive_ptr_release(MiniEngine::IntrusiveTracked *p)
	{
		if (--(p->mReference) == 0)
		{
			p->releaseObject(p);
		}
	}


} 


#endif  
