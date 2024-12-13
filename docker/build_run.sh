NAME=$1

docker build . -t $NAME

docker run --name ${NAME} -v /mnt/d/PWN/iconv:/mnt -p 3333:3333 -p 7777:7777  -it $NAME /bin/bash

# container_id=$(docker ps -n 1 -q)
# docker cp ${container_id}:/home/ctf/pwn .

# docker system prune -a