FROM centos:7 
ENV REFRESHED_AT 2020-10-24

# Install dependencies
RUN yum install -y python3 libgomp libgfortran5 && yum clean all

# After this soft-link you CAN'T use yum, since yum is written in python2
RUN ln -sf /usr/bin/python3 /usr/bin/python

# If you can access pypi.org fast enought, you don't can install packages without changing mirror
RUN python -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pandas scipy --no-cache-dir

COPY . /app

ENV LILLYMOL_HOME=/app
ENV BUILD_DIR=Linux-gcc-8.3.1
ENV BIN_DIR=${LILLYMOL_HOME}/bin/${BUILD_DIR}

ENV PATH="$BIN_DIR:${PATH}"
ENV PYTHONPATH=${LILLYMOL_HOME}/contrib/script/py

RUN ln -sf ${LILLYMOL_HOME}/bin/${BUILD_DIR} ${LILLYMOL_HOME}/bin/Linux
