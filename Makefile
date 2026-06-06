CC = gcc
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

%: %.c
	$(CC) $< -o $@ $(LIBS)
