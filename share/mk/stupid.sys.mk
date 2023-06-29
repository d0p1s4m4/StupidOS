# File: stupid.sys.mk

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.cxx
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.C
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
