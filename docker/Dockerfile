FROM ubuntu:noble-20240423

RUN apt-mark hold libc6
RUN apt update && apt upgrade -y
RUN apt install xinetd gdbserver -y
RUN apt install gcc make -y

WORKDIR /mnt/
COPY ./poc.c .
COPY ./makefile .
COPY ./poc .
# RUN make poc

COPY ./start.sh .
COPY ./debug /etc/xinetd.d/

RUN chmod 755 ./poc ./start.sh

EXPOSE 4444 7777
CMD ["./start.sh"]