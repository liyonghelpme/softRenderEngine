DEBUG = 1
EXECUTABLE = HelloCpp
CXXFLAGS += -Wno-c++11-narrowing

INCLUDES = -I.. -I../Classes -I../../../../extensions/CocoStudio/Json

SOURCES = main.cpp \
        ../Classes/AppDelegate.cpp \
		../Classes/SoftView.cpp \
		../Classes/Mesh.cpp \
		../Classes/Camera.cpp \
		../Classes/util.cpp \
        
		
		
		
#../Classes/HelloWorldScene.cpp

COCOS_ROOT = ../../../..
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
