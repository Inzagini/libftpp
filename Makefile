NAME := libftpp.a

CXX := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -I. -Iincludes
AR := ar rcs
RM := rm -rf

SRCS := \
	srcs/DataStructure/DataBuffer/data_buffer.cpp \
	srcs/DesignPatterns/Memento/memento.cpp \
	srcs/IOStream/Log/log.cpp \
	srcs/IOStream/ThreadSafeIOStream/thread_safe_iostream.cpp \
	srcs/Mathematics/IVector2/ivector2.cpp \
	srcs/Mathematics/IVector3/ivector3.cpp \
	srcs/Mathematics/PerlinNoise2d/perlin_noise_2D.cpp \
	srcs/Mathematics/Random2DCoordinateGenerator/random_2D_coordinate_generator.cpp \
	srcs/Network/Client/client.cpp \
	srcs/Network/Message/message.cpp \
	srcs/Network/Server/server.cpp \
	srcs/Tester/printer.cpp \
	srcs/Tester/runner.cpp \
	srcs/Thread/PersistentWorker/persistent_worker.cpp \
	srcs/Thread/Thread/thread.cpp \
	srcs/Thread/WorkerPool/worker_pool.cpp \
	srcs/Timer/timer.cpp

OBJDIR := obj
OBJS := $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $@ $^

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

-include $(DEPS)

.PHONY: all clean fclean re
