FROM ubuntu:24.04

RUN apt-mark hold libc6
RUN apt update && apt upgrade -y
RUN apt install xinetd gdbserver -y
RUN apt install gcc make net-tools -y

WORKDIR /
COPY ./pwn /

COPY ./start.sh /
COPY ./debug /etc/xinetd.d/

RUN chmod 755 /pwn /start.sh

EXPOSE 3333 9999
CMD ["./start.sh"]
