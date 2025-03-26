FROM ubuntu:24.10

COPY ./src/ /

WORKDIR /fprime_app

RUN apt update

RUN apt-get -y install cmake git python3-pip

RUN pip install -r /fprime_app/fprime/requirements.txt --break-system-packages

COPY ./scripts/ /scripts/
RUN chmod +x /scripts/run.sh

ENTRYPOINT ["/scripts/run.sh"]