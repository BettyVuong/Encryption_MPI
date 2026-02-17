CC = gcc
CFLAGS = -Wall -std=c11 -g
MPICC = mpicc

TARGET1 = a2encrypt
TARGET2 = a2decrypt
TARGET3 = a2decrypt_serial

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1):
	$(CC) $(CFLAGS) -o $(TARGET1) a2encrypt.c

$(TARGET2):
	$(MPICC) $(CFLAGS) -o $(TARGET2) a2_decrypt.c

$(TARGET3):
	$(CC) $(CFLAGS) -o $(TARGET3) a2_decrypt_serial.c

clean:
	rm -rf $(TARGET1) $(TARGET2) $(TARGET3)