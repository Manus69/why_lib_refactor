lib = libwhy.a
lib_header = why.h
driver_name = test

cc = gcc
flags = -Wall -Wextra -g #-fsanitize=address
driver_flags = $(flags)
release_flags = -Wall -Wextra -O3 -flto
release_driver_flags = $(release_flags)

src_dir = src/
obj_dir = obj/
include_dir = src/

headers = $(wildcard $(src_dir)*.h)
source = $(wildcard $(src_dir)*.c)
objects = $(subst $(src_dir),$(obj_dir),$(source:.c=.o))
driver = test.c

.PHONY: directory

# all: directory $(lib)
all: directory $(driver_name)

directory:
	@mkdir -p $(obj_dir)

release: flags = $(release_flags)
release: driver_flags = $(release_driver_flags)
release: fclean all

test_release: flags = $(release_flags)
test_release: fclean all $(driver_name)
 
$(obj_dir)%.o : $(src_dir)%.c $(headers) $(lib_header)
	$(cc) $(flags) $< -I $(include_dir) -I. -c -o $@

$(driver_name): $(driver) $(lib)
	$(cc) $(flags) $< -I $(include_dir) -I. -c -o $(obj_dir)$(driver:.c=.o)
	$(cc) $(driver_flags) $(objects) $(obj_dir)$(driver:.c=.o) -o $(driver_name)

$(lib): $(objects)
	ar rcs $(lib) $(objects)

clean:
	rm -f $(objects) $(driver:.c=.o)

fclean: clean
	rm -f $(driver:.c=.o) $(lib)

re: fclean all

