//in main.cpp
#include "MyClass.h"  // defines MyClass 
int main()
{
  MyClass a; // no longer produces an error, because MyClass is defined
  double area = a.Area(5.0,10.);
  std::cout<<"Area: "<<area<<std::endl;
  return 0;
}
