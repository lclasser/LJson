#########################
# Compile Config        #
#########################
CC		= gcc
LD		= ld
AR		= ar
RM		= rm

#########################
# OS Config             #
#########################
OS_INCLUDES	= -I. -I/usr/include
OS_DEFINES	= -D_LINUX
OS_LIBRARY	= -L/usr/lib64 -lc -lm
OS_CFLAG 	= -m64 -fPIC -Wall -Werror -Wno-pointer-sign -finline-functions -fforce-addr -ffunction-cse -finline
OS_ARFLAGS	= -rcs
OS_LDFLAGS	= -m elf_x86_64 -E -Bshareable -Bsymbolic -Bdynamic 
OS_CCOPTS   = -Wall -rdynamic

#########################
# Compile Option        #
#########################
INCLUDES    = $(OS_INCLUDES) -I../include
DEFINES     = $(OS_DEFINES)
LIBRARIES   = $(OS_LIBRARY)
CFLAGS      = $(OS_CFLAG) $(INCLUDES) $(DEFINES)
ARFLAGS		= $(OS_ARFLAGS)
LDFLAGS		= $(OS_LDFLAGS)
CCOPTS	 	= $(OS_CCOPTS)

.SUFFIXES: .o .c
.c.o : $<
	$(CC) $(CFLAGS) -c $(@:.o=.c)

#########################
# Target & Source       #
#########################
ETARGET = 
STARGET	= ../binary/libljsonlibrary.a
DTARGET	= ../binary/libljsonlibrary.so
SOURCES	= \
	ljson_fundamental.c \
	ljson_fundamental_get.c \
	ljson_fundamental_list.c \
	ljson_fundamental_memory.c \
	ljson_fundamental_read.c \
	ljson_fundamental_set.c \
	ljson_fundamental_write.c
OBJECTS     = $(SOURCES:.c=.o)
HEADERS		= 	../include/ljson_fundamental.h

all: $(STARGET) $(DTARGET)

$(STARGET) : $(OBJECTS) $(SOURCES) $(HEADERS)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE]          [$@]"
	@echo "--------------------------------------------------------- "
	$(AR) $(ARFLAGS) $(STARGET) $(OBJECTS)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE:COMPLETE] [$@]"
	@echo "--------------------------------------------------------- "

$(DTARGET) : $(OBJECTS) $(SOURCES) $(HEADERS)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE]          [$@]"
	@echo "--------------------------------------------------------- "
	$(LD) $(LDFLAGS) -o $(DTARGET) $(OBJECTS) $(LIBRARIES)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE:COMPLETE] [$@]"
	@echo "--------------------------------------------------------- "

clean      :
	$(RM) -f $(OBJECTS)
	$(RM) -f $(STARGET)
	$(RM) -f $(DTARGET)
	$(RM) -f $(ETARGET)
	@echo "--------------------------------------------------------- "
	@echo "- All objects has been removed."
	@echo "--------------------------------------------------------- "

