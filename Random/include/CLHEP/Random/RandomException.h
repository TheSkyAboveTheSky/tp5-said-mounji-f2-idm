#ifndef __RANDOMEXCEPTION__
#define __RANDOMEXCEPTION__


#include <string>

class RandomException
{
   public:
     RandomException(){};
     RandomException(const std::string inMsg){msg = inMsg;}
     std::string msg;
};


class NotImplementedException : public RandomException
{
   public:
     NotImplementedException() : RandomException() {};
     NotImplementedException(const std::string inMsg) : RandomException(inMsg) {}
};


class EngineNotFoundException : public RandomException
{
   public:
     EngineNotFoundException() : RandomException() {};
     EngineNotFoundException(const std::string inMsg) : RandomException(inMsg) {}
};



#endif
