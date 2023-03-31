FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list && \
    apt-get update && apt install -y net-tools

COPY ibn /home/ibn/

WORKDIR /home/ibn/build/bin/

ENTRYPOINT ["/home/ibn/build/bin/start.sh", "-b"]

CMD ["server"]
