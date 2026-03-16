all: main child_execv child_execl child_execlp child_execle

main: main.c
	gcc -o main main.c

child_execv: child_execv.c
	gcc -o child_execv child_execv.c

child_execl: child_execl.c
	gcc -o child_execl child_execl.c

child_execlp: child_execlp.c
	gcc -o child_execlp child_execlp.c

child_execle: child_execle.c
	gcc -o child_execle child_execle.c

clean:
	rm -f main child_execv child_execl child_execlp child_execle
