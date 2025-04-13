FROM ubuntu:noble
ARG TARGETARCH

WORKDIR /master-server

RUN groupadd alterware-master && useradd -r -g alterware-master alterware-master

RUN mkdir -p /master-server && \
    chown alterware-master:alterware-master /master-server && \
    chmod 775 /master-server

COPY --chown=alterware-master:alterware-master --chmod=755 ./linux-${TARGETARCH}-release/alterware-master /master-server

USER alterware-master

EXPOSE 20810/udp

ENV AW_STATS_LOCATION=""

ENTRYPOINT ["/master-server/alterware-master"]
