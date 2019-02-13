NM			= ft_nm
OTOOL   	= ft_otool
SRCS_NM		= src/nm/main.c src/nm/handle_64.c
SRCS_OTOOL 	= src/otool/main.c

OBJS_NM		= 	$(patsubst srcs/%.c,objs/%.o,$(SRCS_NM))
OBJS_OT		= 	$(patsubst srcs/%.c,objs/%.o,$(SRCS_OTOOL))

CC			= 	gcc
CFLAGS		= 	-Wall -Wextra -Werror
INC			= 	-I./include/
LIBFT 		= 	lib/libft/libft.a

CG = \033[92m
CY = \033[93m
CE = \033[0m

all:		$(NM) $(OTOOL)

$(NM):	$(OBJS_NM)
			@ make -C ./lib/libft all
			@ $(CC) $(LIBFT) -o $@ $^
			@ echo "\n\033[92m---> ft_nm program created ✓\033[0m";

$(OTOOL):	$(OBJS_OT)
			@ make -C ./lib/libft all
			@ $(CC) $(LIBFT) -o $@ $^
			@ echo "\n\033[92m---> ft_otool program created ✓\033[0m";

objs/%.o:	srcs/%.c
			@ mkdir -p objs
		 	@ $(CC) $(INC) -c $< -o $@
		 	@ echo "\033[K$(CY)[RT] :$(CE) $(CG)Compiling $<$(CE) \033[1A";

clean:		
			@ /bin/rm -rf objs/
			@ make -C ./lib/libft clean
			@ echo "\033[1;33m---> All .o files cleared\033[0m \033[92m✓\033[0m";

fclean:		clean
			@ /bin/rm -f $(NM)
			@ /bin/rm -f $(OTOOL)
			@ echo "\n\033[1;33m---> Everything cleared\033[2;00m \033[92m✓\033[0m";
re : fclean all

.PHONY: all, clean, fclean, re