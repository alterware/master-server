FROM alpine:latest

RUN apk add --no-cache gcompat libstdc++

COPY --chmod=755 ./linux-x64-release/alterware-master /usr/local/bin/

RUN addgroup -S alterware-master && adduser -S alterware-master -G alterware-master
USER alterware-master

EXPOSE 20810/udp

ENV AW_STATS_LOCATION=""

ENTRYPOINT ["/usr/local/bin/alterware-master"]
