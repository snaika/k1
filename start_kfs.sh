make clean -C perso/kfs/
make -C perso/kfs/
rm *~
qemu-system-x86_64 -kernel perso/kfs/kfs -serial stdio
