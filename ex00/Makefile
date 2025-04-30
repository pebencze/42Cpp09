NAME = btc
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = BitcoinExchange.cpp main.cpp
OBJ_DIR = objs
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
INCL = BitcoinExchange.hpp

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp $(INCL)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all