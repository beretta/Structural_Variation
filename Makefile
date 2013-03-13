# map_analyzer

SRC_DIR:=src
OBJ_DIR:=obj
BIN_DIR:=bin

CFLAGS+= -g -Wall -O2 -DNDEBUG -march=native
CXXFLAGS+= ${CFLAGS}
LIBS = 

.PHONY: all
all:action map_analyzer

.PHONY: action
action:
	@echo "Compiling..."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	echo '* Compiling $<'; \
	mkdir -pv $(dir $@) ; \
	$(CXX) $(CXXFLAGS) -o $@ -c $<

map_analyzer_OBJS= \
	${OBJ_DIR}/main.o \
	${OBJ_DIR}/interval.o \
	${OBJ_DIR}/parse_alignment.o \
	${OBJ_DIR}/count_alignment.o \
	${OBJ_DIR}/common_interval.o \
	${OBJ_DIR}/chr_separation.o \
	${OBJ_DIR}/build_regions.o \

${BIN_DIR}/map_analyzer: ${map_analyzer_OBJS}
	@echo 'Linking $@'; \
	mkdir -p ${BIN_DIR}; \
	${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

.PHONY: map_analyzer
map_analyzer: ${BIN_DIR}/map_analyzer

.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -f ${OBJ_DIR}/* ${BIN_DIR}/*
