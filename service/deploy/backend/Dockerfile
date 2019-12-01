FROM ubuntu:18.04

RUN apt update && \
    apt install -y socat sqlite3 libsqlite3-dev

VOLUME ["/backend"]

WORKDIR /backend

ENTRYPOINT ["./server.sh"]