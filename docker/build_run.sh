docker build . -t

docker run --name -v /home/dbgbgtf/attachment/:/mnt -p 3333:3333 -p 9999:9999  -it container_name /bin/bash

# container_id=$(docker ps -n 1 -q)
# docker cp ${container_id}:/home/ctf/pwn .
# docker system prune -a
