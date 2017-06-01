/*!
 * \file Engine_Functor.h
 * \date 2015/05/01 15:33
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T> void operator () (T& pInstance)
		{			
			if(nullptr != pInstance)
			{
				delete pInstance;
				pInstance = nullptr;
			}
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void){}
		~CDeleteMap(void){}
	public: // operator	
		template <typename T> void operator () (T& Pair)
		{			
			if(nullptr != Pair.second)
			{
				delete Pair.second;
				Pair.second = nullptr;
			}
		}
	};
}

#endif // Engine_Functor_h__