/* Copyright 1990-2008, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
// utilities for JFE to load JE, initiallize, and run profile sentence
// JFEs are jconsole, jwdw, and jwdp
// TODO: Remove all void* where applicable when other parts of the code start being refactored
#include <string>
#include <stdexcept>
#include "util/stringx.h"
#include <unistd.h>
#include <dlfcn.h>
#include "j.h"
#include "jversion.h"
#include <cstdint>

#ifdef __MACH__
    #include <mach-o/dyld.h> // for  _NSGetExecutablePath
    constexpr std::string_view JDLLNAME {"libj.dylib"};
#else
    #include <sys/utsname.h>
    constexpr std::string_view JDLLNAME {"libj.so"};
#endif

extern "C" {

    static void* hjdll; // handle to J DLL
    static J jt;
    static JDoType jdo;
    static JFreeType jfree;
    static JgaType jga;
    static JGetLocaleType jgetlocale;
    static JGetAType jgeta;
    static JSetAType jseta;
    std::string path;
    std::string pathdll;
    static char jdllver[20]; // Not sure why this is being added, but keeping it for now.
    static int FHS=0; // Not sure what this is


    auto jedo(char const *sentence) -> int
    {
        return jdo(jt,reinterpret_cast<C*>(const_cast<char *>(sentence)));
    }

    auto jegeta(I n, char* s) -> A {
        return jgeta(jt,n,reinterpret_cast<C*>(s));
    }

    auto jeseta(I n,char* name,I x,char* d) -> I {
        return jseta(jt,n,reinterpret_cast<C*>(name),x,reinterpret_cast<C*>(d));
    }

    auto jefree() -> void{
        jfree(jt);
    }

    auto jegetlocale() -> char* {
        return reinterpret_cast<char*>(jgetlocale(jt));
    }

    auto jega(I t, I n, I r, I*s) -> A {
        return jga(jt,t,n,r,s);
    }

    auto je_load_procedure_addresses(void* hjdll, void* callbacks) -> void {
        auto jsm = reinterpret_cast<JSMType>(dlsym(hjdll,"JSM"));
        jsm(jt,callbacks);
        jdo        = reinterpret_cast<JDoType>           (dlsym(hjdll,"JDo"));
        jfree      = reinterpret_cast<JFreeType>         (dlsym(hjdll,"JFree"));
        jga        = reinterpret_cast<JgaType>           (dlsym(hjdll,"Jga"));
        jgetlocale = reinterpret_cast<JGetLocaleType>    (dlsym(hjdll,"JGetLocale"));
        jgeta      = reinterpret_cast<JGetAType>         (dlsym(hjdll,"JGetA"));
        jseta      = reinterpret_cast<JSetAType>         (dlsym(hjdll,"JSetA"));
    }

    // load JE, Jinit, getprocaddresses, JSM
    auto jeload(void* callbacks) -> J {
        hjdll = dlopen(pathdll.c_str(),RTLD_LAZY);

        if(!hjdll) {
            char* error = dlerror();
            printf( "ERROR\tCould not open library globally: %s\n", error ? error : "" );
            return nullptr;
        }

        jt = static_cast<JST*>(reinterpret_cast<JInitType>(dlsym(hjdll,"JInit"))());
        if (!jt)
            return nullptr;
        je_load_procedure_addresses(hjdll, callbacks);
        return jt;
    }

    // set path and pathdll (wpath also set for win)
    // WIN arg is 0, Unix arg is argv[0]
    auto jepath(char* arg,char* lib) -> void
    {
        uint32_t const sz  = 4000;
        uint32_t len = sz; // Cant be const for function call _NSGetExecutablePath

        // C strings need to be used for POSIX APIs and macOS APIs
        auto arg2 = new char[sz];
        auto arg3 = new char[sz];
        auto path_temp = new char[sz];
        // Return for readlinks
        int n;

        // try host dependent way to get path to executable
        // use arg if they fail (arg command in PATH won't work)
        #ifdef __MACH__
            // Returns 0 if path was copied, otherwise -1 if failed.
            if(_NSGetExecutablePath(arg2,&len) != 0)
                strcat(arg2,arg);
        #else
        n = readlink("/proc/self/exe",arg2,sz);
        if(n == -1)
            strcpy(arg2,arg);
        else
            arg2[n]=0;
        #endif
        // arg2 is path (abs or relative) to executable or soft link
        n = readlink(arg2,arg3,sz);

        if(n == -1)
            strcpy(arg3,arg2);
        else
            arg3[n]=0;

        if('/'==*arg3)
            strcpy(path_temp,arg3);
        else
        {
            if(!getcwd(path_temp,sizeof(path_temp)))
                path_temp[0]=0;
            strcat(path_temp,"/");
            strcat(path_temp,arg3);
        }
        // Now append path_temp to path, as all POSIX and macOS API calls are done, and free up arg2, arg3, path_temp.
        path.append(path_temp);
        delete [] path_temp;
        delete [] arg2;
        delete [] arg3;

        // Remove everything after the last / as that would be the current executables name
        path.erase(std::next(path.begin(),path.rfind('/')),path.end());

        // remove ./ and backoff ../
        util::remove_all_occurrences(path,"../");
        util::remove_all_occurrences(path,"./");

        pathdll.append(path);
        pathdll.append("/");
        pathdll.append(JDLLNAME);

        if(*lib){
            if('/' != *lib){
                pathdll.append(path);
                pathdll.append("/");
            }
            pathdll.append(lib);
        }

    }

    // called by jwdp (java jnative.c) to set path
    auto jesetpath(char* arg) -> void
    {
        pathdll.append(arg);
        path.append(arg);
        path.erase(std::next(path.begin(),path.rfind('/')),path.end());
    }

    // build and run first sentence to set BINPATH, ARGV, and run profile
    // arg is command line ready to set in ARGV_z_
    // type is 0 normal, 1 -jprofile xxx, 2 ijx basic, 3 nothing
    // profile[ARGV_z_=:...[BINPATH=:....
    // profile is from BINPATH, ARGV, ijx basic, or nothing
    auto jefirst(int type, char *arg) -> int {
        std::string input;

        if (type == 0) {
            if (!FHS)
                input.append("(3 : '0!:0 y')<BINPATH,'");
            else {
                input.append("(3 : '0!:0 y')<'/etc/j/");
                input.append(jdllver);
            }
            input.append("/profile.ijs'");
        } else if (type == 1)
            input.append("(3 : '0!:0 y')2{ARGV");
        else if (type == 2)
            input.append("");
        else
            input.append("i.0 0");

        input.append("[ARGV_z_=:");
        input.append(arg);

        #if defined(__MACH__)
            input.append("[UNAME_z_=:'Darwin'");
        #endif

        input.append("[BINPATH_z_=:'");
        input.append(path);
        input.append("'[LIBFILE_z_=:'");
        input.append(pathdll);
        input.append("'");

        // TODO: When jedo is refactored, change this
        return jedo(input.c_str());
    }

    auto jefail() noexcept(false) -> void {
        throw std::invalid_argument("Load library " + std::string(pathdll) + " failed: " + std::string(strerror(errno)));
    }

}