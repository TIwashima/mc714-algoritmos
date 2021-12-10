
all: build send

build: 
	mpiCC *.cpp -o exec

send:
	scp exec ubuntu@serv02:~
	scp exec ubuntu@serv03:~

run: all
	mpirun -host localhost,serv02,serv03 ./exec

clear:
	rm ./exec