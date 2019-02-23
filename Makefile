CC=gcc
CFLAGS = -Wall

program: clear
	$(CC) $(CFLAGS) comando.c main.c -o notebook

comando: clear
	$(CC) $(CFLAGS) comando.c -o comando

clean:
	rm exec

clear:
	clear

cm: st up
	git add -A 
	git commit -m "update"
	git push
st:
	git status

up:
	git pull


