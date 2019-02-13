NM			= ft_nm
OTOOL   	= ft_otool
SRCS_NM		= src/nm/main.c src/nm/handle_64.c
SRCS_OTOOL 	= src/otool/main.c

OBJS_NM		= 	$(patsubst srcs/%.c,objs/%.o,$(SRCS_NM))
OBJS_OT		= 	$(patsubst srcs/%.c,objs/%.o,$(SRCS_OTOOL))

CC			= 	gcc
CFLAGS		= 	-Wall -Wextra -Werror
INC			= 	-I./include/
PRINTF		= 	lib/ft_printf/libftprintf.a

CG = \033[92m
CY = \033[93m
CE = \033[0m

all:	$(NM) $(OTOOL)

$(NM):	$(OBJS_NM)
			@ make -C ./lib/ft_printf all
			@ $(CC) $(PRINTF) -o $@ $^
			@ echo "\n\033[92m---> ft_nm program created ✓\033[0m";

$(OTOOL):	$(OBJS_OT)
			@ $(CC) $(PRINTF) -o $@ $^
			@ echo "\n\033[92m---> ft_otool program created ✓\033[0m";

objs/%.o:	srcs/%.c
			@ mkdir -p objs
		 	@ $(CC) $(INC) -c $< -o $@
		 	@ echo "\033[K$(CY)[RT] :$(CE) $(CG)Compiling $<$(CE) \033[1A";

clean:		
			@ /bin/rm -rf objs/
			@ make -C ./lib/ft_printf clean
			@ echo "\033[1;33m---> All .o files cleared\033[0m \033[92m✓\033[0m";

fclean:		clean
			@ /bin/rm -f $(NM)
			@ /bin/rm -f $(OTOOL)
			@ make -C ./lib/ft_printf fclean
			@ echo "\n\033[1;33m---> Everything cleared\033[2;00m \033[92m✓\033[0m";
re : fclean all

.PHONY: all, clean, fclean, re