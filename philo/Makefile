# コンパイラとフラグ
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

# ディレクトリパス
SRC_DIR = sources
INC_DIR = includes

# ヘッダーファイル
INCLUDES = -I$(INC_DIR)

# ソースファイル - 実際のファイル構成に合わせて手動で列挙
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

# オブジェクトファイル
OBJS = $(SRCS:.c=.o)

# 実行ファイル名
NAME = philo

# デフォルトターゲット
all: $(NAME)

# オブジェクトファイルの生成ルール
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 実行ファイルの生成ルール
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "コンパイル完了: $(NAME)"

# クリーンルール
clean:
	rm -f $(OBJS)
	@echo "オブジェクトファイルを削除しました"

# フルクリーンルール
fclean: clean
	rm -f $(NAME)
	@echo "実行ファイルを削除しました"

# 再ビルドルール
re: fclean all

# phonyターゲット
.PHONY: all clean fclean re
