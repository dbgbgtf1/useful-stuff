all: exit_func tls

exit_func: exit_func_demo.c
	gcc -fstack-protector-all -pie -z now -z noexecstack -g exit_func_demo.c -o exit_func
	patchelf --set-interpreter ./ld-linux-x86-64.so.2 ./exit_func
	patchelf --replace-needed libc.so.6 ./libc.so.6 ./exit_func

tls: tls_demo.c
	gcc -fstack-protector-all -pie -z now -z noexecstack -g tls_demo.c -o tls
	patchelf --set-interpreter ./ld-linux-x86-64.so.2 ./tls
	patchelf --replace-needed libc.so.6 ./libc.so.6 ./tls