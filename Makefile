NAME = webserv

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP \
           -I./headers \
           -I./headers/Exception  \
		   -I./headers/Config \
		   -I./headers/InstanceInterface \
		   -I./headers/Interface \

OBJDIR = object

SRCS = ./srcs/main.cpp \
	   ./srcs/Utils.cpp \
	   ./srcs/Webserv/Webserv.cpp 

CONFIG = ./srcs/Config/Config.cpp

EXCEPTION = ./srcs/Exception/ConfigFile.cpp

IS = ./srcs/IS/InstanceInterface.cpp

SRCS += $(CONFIG) $(EXCEPTION) $(IS)

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

DEPS = $(OBJS:.o=.d)

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

-include $(DEPS)

.PHONY: all clean fclean re banner