CC = g++

TARGET = app

OUT_DIR = bin/
BINARIES = $(patsubst src/%.cpp,${OUT_DIR}%.o, $(wildcard src/*.cpp))

# Remove -g if not in debug mode
STATIC_LIBS_FLAGS = -L../core -lcore
LIBS_FLAGS = -I./../core -I./src -pthread -lPocoNet -lPocoFoundation -lPocoUtil
COMPILE_FLAGS = -Wall -g -Werror -std=c++17

MKDIR_P = mkdir -p

all: directory ../${TARGET}

run: directory ../${TARGET}
	../${TARGET} --config examples/config.xml

directory: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

../${TARGET}: $(BINARIES)
	$(CC) $(COMPILE_FLAGS) $^ -o $@ $(LIBS_FLAGS) ${STATIC_LIBS_FLAGS}

${OUT_DIR}%.o: src/%.cpp
	$(CC) -c -o $@ $(LIBS_FLAGS) $(COMPILE_FLAGS) $^

clean:
	rm -rf ${OUT_DIR} ../app

.PHONY: all test clean directory
