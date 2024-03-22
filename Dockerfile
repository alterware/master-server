FROM ubuntu:latest

RUN apt-get update
RUN apt-get install -y libc++-dev libcurl4-gnutls-dev

COPY --chmod=755 ./linux-x64-release/alterware-master /usr/local/bin/

RUN groupadd alterware-master && useradd -r -g alterware-master alterware-master
USER alterware-master

EXPOSE 20810/udp

ENTRYPOINT ["/usr/local/bin/alterware-master"]
