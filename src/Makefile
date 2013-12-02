CXX			= g++
CXXFLAGS	= -O2 -Wall -Wextra -std=gnu++11
SRCS		= main.cpp state.cpp record.cpp board.cpp captured_piece.cpp piece.cpp
OBJS 		:= $(SRCS:.cpp=.o) #SRCSの各ファイルのサフィックスの.cppを.oに変換
LIBS		= -lboost_thread-mgw48-mt-s-1_55 -lboost_system-mgw48-mt-s-1_55 -lboost_log-mgw48-mt-s-1_55 -lboost_log_setup-mgw48-mt-s-1_55 -lboost_filesystem-mgw48-mt-s-1_55
TARGET		= AnimalShogi

all:		$(TARGET)

$(TARGET):	$(OBJS)
			$(CXX) -o $@ $(OBJS) $(LIBS)

clean:;		rm -f $(TARGET) $(OBJS)