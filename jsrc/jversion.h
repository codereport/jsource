#define jversion "902"
#if defined(__MACH__)
#define jplatform "darwin"
#elif defined(__linux__)
#define jplatform "linux"
#else
#define jplatform "unknown"
#endif
#define jtype "beta"  // release,beta,... may include bug level such as beta-3
#define jlicense "GPL3"
#define jbuilder "unknown"  // website or email
