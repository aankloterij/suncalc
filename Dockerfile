FROM python:3.5-alpine

MAINTAINER jemoeder

RUN pip3 install matplotlib

COPY . /app

WORKDIR /app

CMD python3 sunplot.py
