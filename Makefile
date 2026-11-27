CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -MMD -MP -I. -I./src -I./Types -I./Assets -I./Scenario -I./Character -I./Obstacles -I./Game -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

BUILD_DIR = build
TARGET = $(BUILD_DIR)/while_guloso

SRCS = src/main.cpp \
       Game/jogo.cpp \
       Obstacles/obstaculos.cpp \
       Assets/arquivos.cpp \
       Scenario/desenho_cenario.cpp \
       Character/guloso.cpp

OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run
