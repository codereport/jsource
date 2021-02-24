/* Copyright 1990-2006, Jsoftware Inc.  All rights reserved.               */
/* Licensed use only. Any other use is in violation of copyright.          */
/*                                                                         */
/* Xenos: Host Command Facilities                                          */

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <spawn.h>

#include "j.h"
#include "x.h"

// return string indicating which JEs this hardware would run
// ""         would run j.dll
// "avx"      would run j.dll or javx.dll
// "avx avx2" would run j.dll or javx.dll or javx2.dll
A
jtjgetx(J jt, A w) {
    return jtcstr(jt, "");
}

A
jthost(J jt, A w) {
    A z;
    F1RANK(1, jthost, UNUSED_VALUE);
    RZ(w = jtvslit(jt, w));
    {
        A t;
        I b = 0;
        C *fn, *s;
        F f;
        I n;
        n = AN(w);
        GATV0(t, LIT, n + 5 + L_tmpnam + 1, 1);
        s  = CAV(t);  // +1 for trailing nul
        fn = 5 + n + s;
        memcpy(s, AV(w), n);
        memcpy(n + s, "   > ", 5L);
        strcpy(fn, "/tmp");
        strcat(fn, "/tmp.XXXXXX");
        {
            int fd = mkstemp(fn);
            close(fd);
        }
#if (defined(__MACH__) && !defined(TARGET_IOS))
        /* no posix_spawn */
        b = !system(s);
#else
        // system() is deprecated
        extern char **environ;
        *(n + s) = 0; /* use action to redirect */
        int status;
        pid_t pid;
        posix_spawn_file_actions_t action;
        posix_spawn_file_actions_init(&action);
        posix_spawn_file_actions_addopen(&action, 1, fn, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        char *argv[] = {"/bin/sh", "-c", NULL, NULL};
        argv[2]      = s;
        if (!(status = posix_spawn(&pid, argv[0], &action, NULL, &argv[0], environ))) {
#if defined(__MACH__)
            /* macos different behavior (from linux) for SIGCHLD */
            /* no interface error will be reported */
            waitpid(pid, &status, 0);
            b = WIFEXITED(status) && !WEXITSTATUS(status);
#else
            if (-1 != waitpid(pid, &status, 0)) b = WIFEXITED(status) && !WEXITSTATUS(status);
#endif
        }
        posix_spawn_file_actions_destroy(&action);
#endif
        if (b) {
            f = fopen(fn, FREAD_O);
            z = jtrd(jt, f, 0L, -1L);
            fclose(f);
        }
        unlink(fn);
        ASSERT(b && f, EVFACE);
    }
    return z;
}

A
jthostne(J jt, A w) {
    F1RANK(1, jthostne, UNUSED_VALUE);
    RZ(w = jtvslit(jt, w));
    {
        I b = system(CAV(jtstr0(jt, w)));
        b   = !b;
        ASSERT(b != 0, EVFACE);
    }
    return mtv;
}

#define CL(f)        \
    {                \
        close(f[0]); \
        close(f[1]); \
    }

A
jthostio(J jt, A w) {
    C* s;
    A z;
    F* pz;
    int fi[2], fo[2], r;
    int fii[2], foi[2];
    if (pipe(fi) == -1) ASSERT(0, EVFACE);
    if (pipe(fo) == -1) {
        CL(fi);
        ASSERT(0, EVFACE);
    }
    fii[0] = fi[0];
    fii[1] = fi[1];
    foi[0] = fo[0];
    foi[1] = fo[1];
    F1RANK(1, jthostio, UNUSED_VALUE);
    RZ(w = jtvs(jt, w));
    s = CAV(jtstr0(jt, w));
    GAT0(z, INT, 3, 1);
    pz = (F*)AV(z);
    if ((r = pipe(fii)) == -1 || pipe(foi) == -1) {
        if (r != -1) CL(fi);
        ASSERT(0, EVFACE);
    }
    if (!((pz[1] = fdopen(fi[0], "r")) && (pz[2] = fdopen(fo[1], "w")))) {
        if (pz[1]) fclose(pz[1]);
        CL(fi);
        CL(fo);
    }
    if (!jtadd2(jt, pz[1], pz[2], s)) {
        fclose(pz[1]);
        fclose(pz[2]);
        CL(fi);
        CL(fo);
    }
    switch (r = fork()) {
        case -1:
            CL(fi);
            CL(fo);
            ASSERT(0, EVFACE);
        case 0:
            close(0);
            { int i = dup(fo[0]); };
            close(1);
            { int i = dup(fi[1]); };
            CL(fi);
            CL(fo);

            execl("/bin/sh", "/bin/sh", "-c", s, NULL);
            exit(-1);
    }
    close(fo[0]);
    close(fi[1]);
    jtadd2(jt, NULL, NULL, NULL);
    pz[0] = (F)(intptr_t)r;
    return z;
}

A
jtjwait(J jt, A w) {
    I k;
    int s;
    RE(k = jti0(jt, w));
    if (-1 == waitpid(k, &s, 0)) jtjerrno(jt);
    return jtsc(jt, s);
}

/* return errno info from c library */
A
jtcerrno(J jt, A w) {
    C buf[1024], ermsg[1024];
    ASSERTMTV(w);

    if (errno && !strerror_r(errno, ermsg, 1024))
        strcpy(buf, ermsg);
    else
        strcpy(buf, "");
    return link(jtsc(jt, errno), jtcstr(jt, buf));
} /* 2!:8  errno information */
