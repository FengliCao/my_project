MPATH1= ./photo/
MPATH2= ./motor/
CFLAGS=-Wall 
OPENCVLIB=-lopencv_core -lopencv_highgui -lopencv_imgproc
OBJS=$(MPATH1)photo.cpp
CXX=g++

all: app
app: pro_all.o $(MPATH1)photo.o $(MPATH2)init_motor.o $(MPATH2)ctl_motor.o
	$(CXX) $(OPENCVLIB) -lpthread -o app pro_all.o $(MPATH1)photo.o $(MPATH2)init_motor.o $(MPATH2)ctl_motor.o 
pro_all.o:pro_all.cpp
	$(CXX) -c pro_all.cpp
photo.o: $(OBJS)
	$(CXX) -O  $(CFLAGS) $(OPENCVLIB) -c $(OBJS)
init_motor.o: init_motor.cpp 
	$(CXX) -O init_motor.cpp -lm -c
ctl_motor.o:ctl_motor.cpp
	$(CXX) -O ctl_motor.cpp -lm -lpthread -c
clean:
	rm -f *.o app $(MPATH1)*.o $(MPATH2)*.o
run: app
	./app
kill:
	pkill app

