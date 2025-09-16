gcc demo.c -g

patchelf --replace-needed libc.so.6 ~/glibc-all-in-one/libs/2.35-0ubuntu3_amd64/libc.so.6 ./a.out
patchelf --set-interpreter ~/glibc-all-in-one/libs/2.35-0ubuntu3_amd64/ld-linux-x86-64.so.2 ./a.out
