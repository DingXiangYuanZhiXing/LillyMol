FROM centos:7
MAINTAINER DingXiangYuanZhiXing 
ENV REFRESHED_AT 2020-10-24

RUN yum install -y python3 libgomp libgfortran5 && yum clean all
RUN ln -sf /usr/bin/python3 /usr/bin/python
RUN python -m pip install pandas scipy --no-cache-dir

COPY . /LillyMol

ENV LILLYMOL_HOME=/LillyMol
ENV BUILD_DIR=Linux-gcc-8.3.1
ENV BIN_DIR=${LILLYMOL_HOME}/bin/${BUILD_DIR}

ENV PATH="$BIN_DIR:${PATH}"
ENV PYTHONPATH=${LILLYMOL_HOME}/contrib/script/py

RUN ln -sf ${LILLYMOL_HOME}/bin/${BUILD_DIR} ${LILLYMOL_HOME}/bin/Linux