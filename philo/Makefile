CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC_DIR = sources
INC_DIR = includes

INCLUDES = -I$(INC_DIR)

SRCS = $(SRC_DIR)/main.c \
	   $(SRC_DIR)/cleanup/cleanup_resources.c \
	   $(SRC_DIR)/cleanup/end_simulation.c \
	   $(SRC_DIR)/core/fork_management.c \
	   $(SRC_DIR)/core/philosopher_actions.c \
	   $(SRC_DIR)/core/philosopher_routine.c \
	   $(SRC_DIR)/init/ft_memset.c \
	   $(SRC_DIR)/init/init_forks.c \
	   $(SRC_DIR)/init/init_philosophers.c \
	   $(SRC_DIR)/init/init_simulation.c \
	   $(SRC_DIR)/init/helper/ft_atoi.c\
	   $(SRC_DIR)/monitor/death_monitor.c \
	   $(SRC_DIR)/monitor/meal_checker.c \
	   $(SRC_DIR)/semaphore/custom_semaphore.c \
	   $(SRC_DIR)/threading/thread_creation.c \
	   $(SRC_DIR)/utils/simulation_is_over.c \
	   $(SRC_DIR)/utils/print_status.c \
	   $(SRC_DIR)/utils/time_utils.c

OBJS = $(SRCS:.c=.o)

NAME = philo

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "コンパイル完了: $(NAME)"

clean:
	rm -f $(OBJS)
	@echo "オブジェクトファイルを削除しました"

fclean: clean
	rm -f $(NAME)
	@echo "実行ファイルを削除しました"

re: fclean all

.PHONY: all clean fclean re
