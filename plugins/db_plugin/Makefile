#
# Makefile for cod4x 1.7a
# =^.^= Mnaauuu =^.^=
# 

CC = gcc

#add external includes
INCDIRS = 
#add external libraries
LDIRS = -L/usr/lib/i386-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl

CFLAGS = ${INCDIRS} -m32 -Wall -O1 -s -fvisibility=hidden -mtune=core2
LDFLAGS = -m32 ${LDIRS}  

OBJDIR = .objs
DEPDIR = .deps

CFILES = $(wildcard *.c)

COBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(CFILES))
DEPOBJS :=  $(patsubst %.c,$(DEPDIR)/%.d,$(CFILES))

TARGET = $(patsubst %_plugin.c,%_plugin.so,$(wildcard *_plugin.c))


all: $(TARGET)

rebuild: clean all	

clean:
	@rm -rf *.so *.o $(COBJS) $(OBJDIR)  $(DEPDIR)

$(TARGET): $(OBJDIR) $(COBJS)
#	@echo 'Building plugin ($@)'
	@$(CC) $(COBJS) -s -shared -o $@ $(LDFLAGS)
	@echo 'Done plugin: $@'  
	
$(OBJDIR)/%.o: %.c
	@echo "Building: " $(*F).c
	@$(CC) $(FLAGS) $(CFLAGS) -c $(*F).c -o $@ 


# automatically generate dependency rules
$(DEPDIR)/%.d : %.c $(DEPDIR) 
	@$(CC) $(FLAGS) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(OBJDIR)/$(<:.c=.o)" "$<"


$(OBJDIR):
	@mkdir -p $@

$(DEPDIR):
	@mkdir -p $@


-include $(DEPOBJS)
