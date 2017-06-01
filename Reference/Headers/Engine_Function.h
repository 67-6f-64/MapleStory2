#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{
	template <typename T> void Safe_Delete (T& pointer)
	{
		if(NULL != pointer)
		{
			delete pointer;
			pointer = NULL;
		}
	}


	template <typename T> void Safe_Delete_Array (T& pointer)
	{
		if(NULL != pointer)
		{
			delete [] pointer;
			pointer = NULL;
		}
	}


	template <typename T> void Safe_Release (T& pointer)
	{
		if(NULL != pointer)
		{
			pointer->Release();
			pointer = NULL;
		}
	}

	template <typename T> void Safe_Single_Destory (T& pointer)
	{
		if(NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}

}

#endif // Engine_Function_h__
