all:timerTest serverTest logTest threadPoolTest sockTest httpTest
.PHONY : all
incdir = -I. \
		-I./Log \
		-I./Util \
		-I./Reactor/Socket \
		-I./Reactor/Epoll \
		-I./Reactor/EventLoop \
		-I./Timer \
		-I./Http

OBJS = 	Http/HttpResp.o \
		Http/HttpReq.o \
		Http/HttpServer.o \
		Timer/Timer.o \
		Reactor/Socket/Socket.o \
		Reactor/Socket/Handle.o \
		Reactor/Socket/TcpServer.o \
		Reactor/EventLoop/EventLoop.o \
		Reactor/Epoll/EpollPoller.o \
		ThreadPool/ThreadPool.o \
		Log/AppendFile.o \
		Log/LogFile.o \
		Log/LogStream.o \
		Log/Logging.o \
		Log/AsynLog.o \
		Util/Util.o \

CXX = g++
CXXFLAGS = -lpthread -g
TARGETDIR = ./bin/

timerTest:Tests/timerTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)timerTest Tests/timerTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/timerTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

serverTest:Tests/ServerTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)serverTest Tests/ServerTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/ServerTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

logTest:Tests/asynLogTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)logTest Tests/asynLogTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/asynLogTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

threadPoolTest:Tests/ThreadPoolTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)threadPoolTest Tests/ThreadPoolTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/ThreadPoolTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

sockTest:Tests/sockTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)sockTest Tests/sockTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/sockTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

httpTest:Tests/httpTest.o $(OBJS)
	$(CXX) -o $(TARGETDIR)httpTest Tests/httpTest.o $(OBJS) -I$(incdir) $(CXXFLAGS)
Tests/httpTest.o:%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

$(OBJS):%.o:%.cpp
	$(CXX) -c -g $< $(incdir) -o $@

.PHONY:clean
clean:
	rm -f $(TARGETDIR)timerTest \
	$(TARGETDIR)serverTest \
	$(TARGETDIR)logTest \
	$(TARGETDIR)threadPoolTest \
	$(TARGETDIR)sockTest \
	$(TARGETDIR)httpTest \
	Tests/*.o \
	sockTest $(OBJS)

rebuild: clean all