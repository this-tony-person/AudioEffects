# Specify the target executable
TARGET = pa_effectchain

# List the source files
SOURCES = pa_effectchain.c ./effects/*.c ./effects/helperfunctions/*.c 

# Specify the libraries to link
LIBS =   -L/usr/local/lib -lportaudio \
	 -framework CoreServices \
     	 -framework CoreFoundation \
	 -framework AudioUnit \
	 -framework AudioToolbox \
	 -framework CoreAudio

# Build rule
$(TARGET): $(SOURCES)
	gcc -g -o $(TARGET) $(SOURCES) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET)
