#DEBUG_FLAGS = -Os -g
#DEBUG=1
BOARD_TAG = nano
BOARD_SUB = atmega328
#BOARD_TAG = mega
#BOARD_SUB = atmega2560
MONITOR_BAUDRATE=115200
#MONITOR_BAUDRATE=9600
#TARGET = demo
# USER_LIB_PATH= $(ARDMK_DIR)/libs/

DEFINES += -DOUTPUT_TARGET=OUTPUT_TARGET_terminal
CFLAGS += $(DEFINES)
CXXFLAGS += $(DEFINES)
ASFLAGS += $(DEFINES)
include ../Makefile
# include $(ARDMK_DIR)/Arduino.mk


$(TARGET_ELF): $(VERSION_HEADER)

