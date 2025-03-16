FROM ubuntu:24.10

WORKDIR /fprime_cont

RUN apt update

RUN apt-get -y install cmake git python3-pip

COPY ./src/fprime/requirements.txt /

RUN pip install -r "/requirements.txt" --break-system-packages

COPY ./scripts /scripts
COPY ./src /fprime_cont
RUN chmod +x /scripts/run.sh

ENTRYPOINT ["/scripts/run.sh"]