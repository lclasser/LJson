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
INCLUDES    = $(OS_INCLUDES)
DEFINES     = $(OS_DEFINES)
LIBRARIES   = $(OS_LIBRARY) -L../binary -lljsonlibrary
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
ETARGET	= test_centos
STARGET	= 
DTARGET	= 
SOURCES	= main.c 
OBJECTS = $(SOURCES:.c=.o)
HEADERS	= 

all : $(STARGET) $(DTARGET) $(ETARGET)

$(ETARGET)	: $(OBJECTS) $(SOURCES)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE]     [$@]"
	@echo "--------------------------------------------------------- "
	$(CC) $(CCOPTS) -o $@ $(OBJECTS) $(LIBRARIES)
	@echo "--------------------------------------------------------- "
	@echo "- [COMPILE:OK]  [$@]"
	@echo "--------------------------------------------------------- "

clean      :
	$(RM) -f $(OBJECTS)
	$(RM) -f $(STARGET)
	$(RM) -f $(DTARGET)
	$(RM) -f $(ETARGET)
	@echo "--------------------------------------------------------- "
	@echo "- All objects has been removed."
	@echo "--------------------------------------------------------- "

