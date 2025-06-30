NAME = webserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -I./headers -std=c++98

OBJDIR = object

SRCS = srcs/main.cpp 

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: banner $(NAME)

banner:
	@echo "\033[1;36m🔧 Compiling $(NAME)... Please wait\033[0m"

$(NAME): $(OBJS)
	@echo "\033[1;32m✅ Linking $(NAME)\033[0m"
	@$(CC) -o $@ $^
	@echo "\033[1;32m🎉 Build complete!\033[0m"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "\033[1;34mCompiling %-80s\033[0m" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf " [\033[1;32mOK\033[0m]\n"

clean:
	@rm -rf $(OBJDIR)
	@echo "\033[1;33m🧹 Cleaned object files.\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;31m🗑️ Removed binary $(NAME).\033[0m"

re: fclean all

.PHONY: all clean fclean re banner