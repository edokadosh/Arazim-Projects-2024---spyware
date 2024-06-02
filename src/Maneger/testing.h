#ifndef TESTING_H
#define TESTING_H
#ifdef TESTING_MODE

#define IS_TRUE(x) { if (!(x)) std::cerr << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }


#endif
#endif