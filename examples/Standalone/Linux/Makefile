# Tonic Standalone Demo Makefile
#
# Created by @anhinga
#
# 2013/07/02 - created by @anhinga
# 2025/05/30 - @whitetiger45 - add google test variables and targets, credit @ mihaitodor \
# https://gist.github.com/mihaitodor/bfb8e7ad908489fdf3ceb496573f306a
##############################################################
#
# NOTE: Please set this to the Audio API you intend to use.
#
# Each API has its own set of linker dependencies, which are
# automatically added to the linker flags below.
#
# Valid values are:
#	LINUX_ALSA
#	LINUX_PULSE
#	LINUX_OSS
#	UNIX_JACK

AUDIO_API = LINUX_PULSE

###############################################################

# == Variables ==

CC = g++
#CCFLAGS = -m32

RTAUDIO_DIR = ../RTAudio
TONIC_SRC_DIR = ../../../src
TONIC_LIB_DIR = lib
INCLUDE_FLAGS = -I$(TONIC_SRC_DIR) -I$(TONIC_SRC_DIR)/Tonic

# googletest variables
GTEST_DIR=../../../../googletest-main/googletest
TEST_DIR=TonicTests
# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
TESTS = TestTonic demo
# All Google Test headers. Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

TONIC_CPP_FILES := $(wildcard $(TONIC_SRC_DIR)/Tonic/*.cpp)
TONIC_OBJ_FILES := $(addprefix $(TONIC_LIB_DIR)/linux/obj/,$(notdir $(TONIC_CPP_FILES:.cpp=.o)))

# libraries for RtAudio

ifeq ($(AUDIO_API),LINUX_ALSA)
RTAUDIO_LIBS = -lasound -lpthread
endif

ifeq ($(AUDIO_API),LINUX_OSS)
RTAUDIO_LIBS = -lpthread
endif

ifeq ($(AUDIO_API),LINUX_PULSE)
RTAUDIO_LIBS = -lpulse-simple -lpulse -lpthread
endif

ifeq ($(AUDIO_API),UNIX_JACK)
RTAUDIO_LIBS = `pkg-config --cflags --libs jack` -lpthread
endif

# == Tests Targets == 
all : $(TESTS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

TonicTests.o: $(TEST_DIR)/TonicTests.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $(TEST_DIR)/TonicTests.cpp

TestTonic: TonicTests.o gtest_main.a $(TONIC_OBJ_FILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# == Targets ==
demo: libTonicLib.a
	$(CC) $(CCFLAGS) $(INCLUDE_FLAGS) \
	-I$(RTAUDIO_DIR) -D__$(AUDIO_API)__ -L$(TONIC_LIB_DIR)/linux ../TonicStandaloneDemo/main.cpp \
	$(RTAUDIO_DIR)/RtAudio.cpp -lTonicLib $(RTAUDIO_LIBS) -o demo 

libTonicLib.a: $(TONIC_OBJ_FILES)
	ar rc $(TONIC_LIB_DIR)/linux/libTonicLib.a $(TONIC_OBJ_FILES)

$(TONIC_LIB_DIR)/linux/obj/%.o: $(TONIC_SRC_DIR)/Tonic/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c $(INCLUDE_FLAGS) -o $@ $<

clean:
	rm -rf $(TONIC_LIB_DIR) $(TONIC_OBJ_DIR) $(TESTS) demo gtest.a gtest_main.a *.o

