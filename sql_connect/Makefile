WORK_PATH=$(shell pwd)

BIN=sql_connect
SRC=sql_connect.cpp
CC=g++
INCLUDE=-I${WORK_PATH}/mysql_lib/include -I.
LIB_PATH=-L${WORK_PATH}/mysql_lib/lib 

LIB_NAME=-lmysqlclient

$(BIN):$(SRC)
	$(CC) -o $@ $^ ${INCLUDE} ${LIB_PATH} ${LIB_NAME} -D_DEBUG_
	#链接mysqlclient库

.PHONY:clean
clean:
	rm -rf $(BIN)


