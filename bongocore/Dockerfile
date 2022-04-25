FROM ubuntu:20.04

RUN apt update && apt install -y g++ cmake make libpoco-dev && mkdir -p build

WORKDIR /bongocore
COPY . .

RUN cmake .
RUN make bongo

CMD ["./bongo", "--config", "server/examples/config21.xml"]
