FROM ubuntu:latest

RUN apt-get update && apt-get upgrade -yyq
RUN apt-get install socat libseccomp-dev -yyq

ADD --chown=root ./case4 /case4
ADD --chown=root ./flag.txt /tmp/flag.txt
ADD --chown=root ./serve.sh /serve.sh

WORKDIR /

ENTRYPOINT /serve.sh
