FROM python:3.7-alpine

RUN apk add --no-cache socat
RUN pip3 install prettytable

ENV PYTHONUNBUFFERED=1

VOLUME ["/frontend"]

WORKDIR /frontend

ENTRYPOINT ["./server.sh"]
