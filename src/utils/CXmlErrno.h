#ifndef _CXMLERRNO_H__
#define _CXMLERRNO_H__

#ifndef CXML_THREAD_LOCAL__
#   ifndef thread_local
#       if __STDC_VERSION__ >= 201112 && !defined __STDC_NO_THREADS__
#           define CXML_THREAD_LOCAL__ _Thread_local
#       elif defined(_WIN32) && (defined(_MSC_VER) || defined(__ICL) ||         \
                                defined(__DMC__)  || defined(__BORLANDC__))
#           define CXML_THREAD_LOCAL__ __declspec(thread)
#       elif defined(__GNUC__) || defined(__SUNPRO_C) || defined(__xlC__)
#           define CXML_THREAD_LOCAL__ __thread
#       else
#           warning "Cannot define thread_local, cxmlerrno will not be threadsafe"
#       endif
#   else
#       define CXML_THREAD_LOCAL__ thread_local
#   endif
#endif

extern CXML_THREAD_LOCAL__ int cxmlerrno;

//Should be called in beginning of functions in CXml modules
#define CXML_RESET_ERRNO__ cxmlerrno = CXML_ESUCCESS; //TODO Check that it's called

#define CXML_ESUCCESS 0
#define CXML_EFAILURE (-1)
#define CXML_EALLOC (-2)
#define CXML_EDOCCHILD (-3)
#define CXML_ENULLARG (-4)
#define CXML_EOUTBOUNDS (-5)
#define CXML_ERRNO (-100) //TODO Should be removed

#endif // _CXMLERRNO_H__