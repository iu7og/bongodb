CC = g++
AR = ar rvs

TARGET = libcore.a

OUT_DIR = bin/
BINARIES = $(patsubst backend/%.cpp,${OUT_DIR}%.o, $(wildcard backend/*.cpp)) $(patsubst storages/%.cpp,${OUT_DIR}%.o, $(wildcard storages/*.cpp)) $(patsubst clients/%.cpp,${OUT_DIR}%.o, $(wildcard clients/*.cpp)) $(patsubst common/%.cpp,${OUT_DIR}%.o, $(wildcard common/*.cpp))

# Remove -g if not in debug mode
LIBS_FLAGS = -I. -pthread -lPocoUtil
COMPILE_FLAGS = -Wall -g -Werror -std=c++17

MKDIR_P = mkdir -p

all: directory ${TARGET}

directory: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

${TARGET}: $(BINARIES)
	$(AR) $@ $^

${OUT_DIR}%.o: backend/%.cpp
	$(CC) -c -o $@ $(COMPILE_FLAGS) $^ $(LIBS_FLAGS)

${OUT_DIR}%.o: clients/%.cpp
	$(CC) -c -o $@ $(COMPILE_FLAGS) $^ $(LIBS_FLAGS)

${OUT_DIR}%.o: storages/%.cpp
	$(CC) -c -o $@ $(COMPILE_FLAGS) $^ $(LIBS_FLAGS)

${OUT_DIR}%.o: common/%.cpp
	$(CC) -c -o $@ $(COMPILE_FLAGS) $^ $(LIBS_FLAGS)

clean:
	rm -rf ${OUT_DIR} ${TARGET}

.PHONY: all test clean directory
