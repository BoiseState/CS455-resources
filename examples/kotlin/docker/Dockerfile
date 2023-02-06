# docker pull amazoncorretto
# FROM scratch
FROM amazoncorretto:latest

COPY TinyHttpd.jar /bin/
COPY mysecurity.policy /home/
RUN mkdir /home/http_root
COPY http_root/ /home/http_root

WORKDIR run

CMD [ \
    "java", \
    "-Djava.security.policy=/home/mysecurity.policy", \
    "-jar", \
    "/bin/TinyHttpd.jar", \
    "5005", \
    "/home/http_root" \
]
