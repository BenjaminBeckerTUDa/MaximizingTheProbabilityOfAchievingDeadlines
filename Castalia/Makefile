#
# OMNeT++/OMNEST Makefile for CastaliaBin
#
# This file was generated with the command:
#  opp_makemake -f -r --deep -o CastaliaBin -u Cmdenv -P /home/benny/Castalia/Castalia -M release -X Simulations -X out -X bin
#

# Name of target to be created (-o option)
TARGET = CastaliaBin$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
#USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Isrc \
    -Isrc/helpStructures \
    -Isrc/node \
    -Isrc/node/application \
    -Isrc/node/application/simpleAggregation \
    -Isrc/node/communication \
    -Isrc/node/communication/mac \
    -Isrc/node/communication/mac/tMac \
    -Isrc/node/communication/radio \
    -Isrc/node/communication/routing \
    -Isrc/node/communication/routing/PLR \
    -Isrc/node/mobilityManager \
    -Isrc/node/mobilityManager/noMobilityManager \
    -Isrc/node/resourceManager \
    -Isrc/node/sensorManager \
    -Isrc/physicalProcess \
    -Isrc/physicalProcess/customizablePhysicalProcess \
    -Isrc/wirelessChannel \
    -Isrc/wirelessChannel/defaultChannel \
    -Isrc/wirelessChannel/traceChannel

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/helpStructures/CastaliaModule.o \
    $O/src/helpStructures/DebugInfoWriter.o \
    $O/src/helpStructures/TimerService.o \
    $O/src/node/application/VirtualApplication.o \
    $O/src/node/application/simpleAggregation/SimpleAggregation.o \
    $O/src/node/communication/mac/VirtualMac.o \
    $O/src/node/communication/mac/tMac/TMAC.o \
    $O/src/node/communication/radio/Radio.o \
    $O/src/node/communication/radio/RadioSupportFunctions.o \
    $O/src/node/communication/routing/VirtualRouting.o \
    $O/src/node/communication/routing/PLR/CDF.o \
    $O/src/node/communication/routing/PLR/PLR.o \
    $O/src/node/mobilityManager/VirtualMobilityManager.o \
    $O/src/node/mobilityManager/noMobilityManager/NoMobilityManager.o \
    $O/src/node/resourceManager/ResourceManager.o \
    $O/src/node/sensorManager/SensorManager.o \
    $O/src/physicalProcess/customizablePhysicalProcess/CustomizablePhysicalProcess.o \
    $O/src/wirelessChannel/defaultChannel/WirelessChannel.o \
    $O/src/wirelessChannel/defaultChannel/WirelessChannelTemporal.o \
    $O/src/wirelessChannel/traceChannel/TraceChannel.o \
    $O/src/helpStructures/TimerServiceMessage_m.o \
    $O/src/node/application/ApplicationPacket_m.o \
    $O/src/node/application/simpleAggregation/DeadlinePacket_m.o \
    $O/src/node/communication/mac/MacPacket_m.o \
    $O/src/node/communication/mac/tMac/TMacPacket_m.o \
    $O/src/node/communication/radio/RadioControlMessage_m.o \
    $O/src/node/communication/routing/RoutingPacket_m.o \
    $O/src/node/communication/routing/PLR/PLRControl_m.o \
    $O/src/node/communication/routing/PLR/PLRPacket_m.o \
    $O/src/node/mobilityManager/MobilityManagerMessage_m.o \
    $O/src/node/resourceManager/ResourceManagerMessage_m.o \
    $O/src/node/sensorManager/SensorManagerMessage_m.o \
    $O/src/physicalProcess/PhysicalProcessMessage_m.o \
    $O/src/wirelessChannel/WirelessChannelMessages_m.o

# Message files
MSGFILES = \
    src/helpStructures/TimerServiceMessage.msg \
    src/node/application/ApplicationPacket.msg \
    src/node/application/simpleAggregation/DeadlinePacket.msg \
    src/node/communication/mac/MacPacket.msg \
    src/node/communication/mac/tMac/TMacPacket.msg \
    src/node/communication/radio/RadioControlMessage.msg \
    src/node/communication/routing/RoutingPacket.msg \
    src/node/communication/routing/PLR/PLRControl.msg \
    src/node/communication/routing/PLR/PLRPacket.msg \
    src/node/mobilityManager/MobilityManagerMessage.msg \
    src/node/resourceManager/ResourceManagerMessage.msg \
    src/node/sensorManager/SensorManagerMessage.msg \
    src/physicalProcess/PhysicalProcessMessage.msg \
    src/wirelessChannel/WirelessChannelMessages.msg

# Default mode (-M option); can be overridden with make MODE=debug (or =release)
ifndef MODE
MODE = release
endif

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f CastaliaBin CastaliaBin.exe libCastaliaBin.so libCastaliaBin.a libCastaliaBin.dll libCastaliaBin.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/helpStructures/*_m.cc src/helpStructures/*_m.h
	$(Q)-rm -f src/node/*_m.cc src/node/*_m.h
	$(Q)-rm -f src/node/application/*_m.cc src/node/application/*_m.h
	$(Q)-rm -f src/node/application/simpleAggregation/*_m.cc src/node/application/simpleAggregation/*_m.h
	$(Q)-rm -f src/node/communication/*_m.cc src/node/communication/*_m.h
	$(Q)-rm -f src/node/communication/mac/*_m.cc src/node/communication/mac/*_m.h
	$(Q)-rm -f src/node/communication/mac/tMac/*_m.cc src/node/communication/mac/tMac/*_m.h
	$(Q)-rm -f src/node/communication/radio/*_m.cc src/node/communication/radio/*_m.h
	$(Q)-rm -f src/node/communication/routing/*_m.cc src/node/communication/routing/*_m.h
	$(Q)-rm -f src/node/communication/routing/PLR/*_m.cc src/node/communication/routing/PLR/*_m.h
	$(Q)-rm -f src/node/mobilityManager/*_m.cc src/node/mobilityManager/*_m.h
	$(Q)-rm -f src/node/mobilityManager/noMobilityManager/*_m.cc src/node/mobilityManager/noMobilityManager/*_m.h
	$(Q)-rm -f src/node/resourceManager/*_m.cc src/node/resourceManager/*_m.h
	$(Q)-rm -f src/node/sensorManager/*_m.cc src/node/sensorManager/*_m.h
	$(Q)-rm -f src/physicalProcess/*_m.cc src/physicalProcess/*_m.h
	$(Q)-rm -f src/physicalProcess/customizablePhysicalProcess/*_m.cc src/physicalProcess/customizablePhysicalProcess/*_m.h
	$(Q)-rm -f src/wirelessChannel/*_m.cc src/wirelessChannel/*_m.h
	$(Q)-rm -f src/wirelessChannel/defaultChannel/*_m.cc src/wirelessChannel/defaultChannel/*_m.h
	$(Q)-rm -f src/wirelessChannel/traceChannel/*_m.cc src/wirelessChannel/traceChannel/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc src/*.cc src/helpStructures/*.cc src/node/*.cc src/node/application/*.cc src/node/application/simpleAggregation/*.cc src/node/communication/*.cc src/node/communication/mac/*.cc src/node/communication/mac/tMac/*.cc src/node/communication/radio/*.cc src/node/communication/routing/*.cc src/node/communication/routing/PLR/*.cc src/node/mobilityManager/*.cc src/node/mobilityManager/noMobilityManager/*.cc src/node/resourceManager/*.cc src/node/sensorManager/*.cc src/physicalProcess/*.cc src/physicalProcess/customizablePhysicalProcess/*.cc src/wirelessChannel/*.cc src/wirelessChannel/defaultChannel/*.cc src/wirelessChannel/traceChannel/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/helpStructures/CastaliaModule.o: src/helpStructures/CastaliaModule.cc \
  src/CastaliaMessages.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/helpStructures/CastaliaModule.h
$O/src/helpStructures/DebugInfoWriter.o: src/helpStructures/DebugInfoWriter.cc \
  src/helpStructures/DebugInfoWriter.h
$O/src/helpStructures/TimerService.o: src/helpStructures/TimerService.cc \
  src/helpStructures/TimerService.h \
  src/CastaliaMessages.h \
  src/helpStructures/TimerServiceMessage_m.h
$O/src/helpStructures/TimerServiceMessage_m.o: src/helpStructures/TimerServiceMessage_m.cc \
  src/helpStructures/TimerServiceMessage_m.h
$O/src/node/application/ApplicationPacket_m.o: src/node/application/ApplicationPacket_m.cc \
  src/node/application/ApplicationPacket_m.h
$O/src/node/application/VirtualApplication.o: src/node/application/VirtualApplication.cc \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/resourceManager/ResourceManager.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/node/application/ApplicationPacket_m.h \
  src/CastaliaMessages.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/communication/mac/MacPacket_m.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/helpStructures/TimerService.h \
  src/node/application/VirtualApplication.h \
  src/node/sensorManager/SensorManagerMessage_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h
$O/src/node/application/simpleAggregation/DeadlinePacket_m.o: src/node/application/simpleAggregation/DeadlinePacket_m.cc \
  src/node/application/ApplicationPacket_m.h \
  src/node/application/simpleAggregation/DeadlinePacket_m.h
$O/src/node/application/simpleAggregation/SimpleAggregation.o: src/node/application/simpleAggregation/SimpleAggregation.cc \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/node/application/simpleAggregation/SimpleAggregation.h \
  src/helpStructures/TimerService.h \
  src/node/communication/mac/MacPacket_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/node/application/VirtualApplication.h \
  src/node/sensorManager/SensorManagerMessage_m.h \
  src/CastaliaMessages.h \
  src/node/application/simpleAggregation/DeadlinePacket_m.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/application/ApplicationPacket_m.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/resourceManager/ResourceManager.h
$O/src/node/communication/mac/MacPacket_m.o: src/node/communication/mac/MacPacket_m.cc \
  src/node/communication/mac/MacPacket_m.h
$O/src/node/communication/mac/VirtualMac.o: src/node/communication/mac/VirtualMac.cc \
  src/node/communication/mac/MacPacket_m.h \
  src/helpStructures/TimerService.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/CastaliaMessages.h \
  src/node/communication/mac/VirtualMac.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/resourceManager/ResourceManager.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/communication/routing/RoutingPacket_m.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/radio/RadioControlMessage_m.h
$O/src/node/communication/mac/tMac/TMAC.o: src/node/communication/mac/tMac/TMAC.cc \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/communication/routing/PLR/PLRControl_m.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/resourceManager/ResourceManager.h \
  src/node/communication/routing/RoutingPacket_m.h \
  src/node/communication/routing/PLR/CDF.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/CastaliaMessages.h \
  src/node/communication/mac/VirtualMac.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/communication/mac/tMac/TMAC.h \
  src/node/communication/routing/PLR/PLRPacket_m.h \
  src/node/communication/mac/MacPacket_m.h \
  src/helpStructures/TimerService.h \
  src/node/communication/mac/tMac/TMacPacket_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h
$O/src/node/communication/mac/tMac/TMacPacket_m.o: src/node/communication/mac/tMac/TMacPacket_m.cc \
  src/node/communication/mac/MacPacket_m.h \
  src/node/communication/mac/tMac/TMacPacket_m.h
$O/src/node/communication/radio/Radio.o: src/node/communication/radio/Radio.cc \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/resourceManager/ResourceManager.h \
  src/CastaliaMessages.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/communication/mac/MacPacket_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h
$O/src/node/communication/radio/RadioControlMessage_m.o: src/node/communication/radio/RadioControlMessage_m.cc \
  src/node/communication/radio/RadioControlMessage_m.h
$O/src/node/communication/radio/RadioSupportFunctions.o: src/node/communication/radio/RadioSupportFunctions.cc \
  src/CastaliaMessages.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/node/communication/radio/RadioSupportFunctions.h
$O/src/node/communication/routing/RoutingPacket_m.o: src/node/communication/routing/RoutingPacket_m.cc \
  src/node/communication/routing/RoutingPacket_m.h
$O/src/node/communication/routing/VirtualRouting.o: src/node/communication/routing/VirtualRouting.cc \
  src/node/application/ApplicationPacket_m.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/helpStructures/CastaliaModule.h \
  src/node/communication/routing/RoutingPacket_m.h \
  src/node/resourceManager/ResourceManager.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/node/communication/routing/VirtualRouting.h \
  src/helpStructures/TimerService.h \
  src/node/communication/mac/MacPacket_m.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/communication/radio/Radio.h \
  src/CastaliaMessages.h
$O/src/node/communication/routing/PLR/CDF.o: src/node/communication/routing/PLR/CDF.cc \
  src/node/communication/routing/PLR/CDF.h
$O/src/node/communication/routing/PLR/PLR.o: src/node/communication/routing/PLR/PLR.cc \
  src/node/communication/routing/PLR/PLRPacket_m.h \
  src/node/communication/radio/Radio.h \
  src/helpStructures/DebugInfoWriter.h \
  src/CastaliaMessages.h \
  src/node/application/simpleAggregation/DeadlinePacket_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/node/communication/mac/MacPacket_m.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/helpStructures/TimerService.h \
  src/node/communication/routing/VirtualRouting.h \
  src/node/communication/routing/RoutingPacket_m.h \
  src/node/communication/routing/PLR/CDF.h \
  src/node/communication/radio/RadioSupportFunctions.h \
  src/node/resourceManager/ResourceManager.h \
  src/node/communication/radio/RadioControlMessage_m.h \
  src/helpStructures/CastaliaModule.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/communication/routing/PLR/PLRControl_m.h \
  src/helpStructures/TimerServiceMessage_m.h \
  src/node/application/ApplicationPacket_m.h \
  src/node/communication/routing/PLR/PLR.h
$O/src/node/communication/routing/PLR/PLRControl_m.o: src/node/communication/routing/PLR/PLRControl_m.cc \
  src/node/communication/routing/PLR/PLRControl_m.h
$O/src/node/communication/routing/PLR/PLRPacket_m.o: src/node/communication/routing/PLR/PLRPacket_m.cc \
  src/node/communication/routing/PLR/PLRPacket_m.h \
  src/node/communication/routing/PLR/CDF.h \
  src/node/communication/routing/RoutingPacket_m.h
$O/src/node/mobilityManager/VirtualMobilityManager.o: src/node/mobilityManager/VirtualMobilityManager.cc \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/helpStructures/DebugInfoWriter.h \
  src/CastaliaMessages.h \
  src/helpStructures/CastaliaModule.h \
  src/wirelessChannel/WirelessChannelMessages_m.h
$O/src/node/mobilityManager/noMobilityManager/NoMobilityManager.o: src/node/mobilityManager/noMobilityManager/NoMobilityManager.cc \
  src/helpStructures/CastaliaModule.h \
  src/node/mobilityManager/noMobilityManager/NoMobilityManager.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/helpStructures/DebugInfoWriter.h \
  src/CastaliaMessages.h
$O/src/node/resourceManager/ResourceManager.o: src/node/resourceManager/ResourceManager.cc \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/CastaliaMessages.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/resourceManager/ResourceManager.h \
  src/helpStructures/CastaliaModule.h
$O/src/node/resourceManager/ResourceManagerMessage_m.o: src/node/resourceManager/ResourceManagerMessage_m.cc \
  src/node/resourceManager/ResourceManagerMessage_m.h
$O/src/node/sensorManager/SensorManager.o: src/node/sensorManager/SensorManager.cc \
  src/CastaliaMessages.h \
  src/helpStructures/DebugInfoWriter.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/node/sensorManager/SensorManagerMessage_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/physicalProcess/PhysicalProcessMessage_m.h \
  src/helpStructures/CastaliaModule.h \
  src/node/sensorManager/SensorManager.h
$O/src/node/sensorManager/SensorManagerMessage_m.o: src/node/sensorManager/SensorManagerMessage_m.cc \
  src/node/sensorManager/SensorManagerMessage_m.h
$O/src/physicalProcess/PhysicalProcessMessage_m.o: src/physicalProcess/PhysicalProcessMessage_m.cc \
  src/physicalProcess/PhysicalProcessMessage_m.h
$O/src/physicalProcess/customizablePhysicalProcess/CustomizablePhysicalProcess.o: src/physicalProcess/customizablePhysicalProcess/CustomizablePhysicalProcess.cc \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/helpStructures/DebugInfoWriter.h \
  src/CastaliaMessages.h \
  src/physicalProcess/customizablePhysicalProcess/CustomizablePhysicalProcess.h \
  src/helpStructures/CastaliaModule.h \
  src/physicalProcess/PhysicalProcessMessage_m.h
$O/src/wirelessChannel/WirelessChannelMessages_m.o: src/wirelessChannel/WirelessChannelMessages_m.cc \
  src/wirelessChannel/WirelessChannelMessages_m.h
$O/src/wirelessChannel/defaultChannel/WirelessChannel.o: src/wirelessChannel/defaultChannel/WirelessChannel.cc \
  src/wirelessChannel/defaultChannel/WirelessChannel.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/wirelessChannel/defaultChannel/WirelessChannelTemporal.h \
  src/helpStructures/CastaliaModule.h \
  src/node/mobilityManager/VirtualMobilityManager.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/CastaliaMessages.h \
  src/helpStructures/DebugInfoWriter.h
$O/src/wirelessChannel/defaultChannel/WirelessChannelTemporal.o: src/wirelessChannel/defaultChannel/WirelessChannelTemporal.cc \
  src/wirelessChannel/defaultChannel/WirelessChannelTemporal.h
$O/src/wirelessChannel/traceChannel/TraceChannel.o: src/wirelessChannel/traceChannel/TraceChannel.cc \
  src/wirelessChannel/defaultChannel/WirelessChannelTemporal.h \
  src/helpStructures/CastaliaModule.h \
  src/wirelessChannel/WirelessChannelMessages_m.h \
  src/node/resourceManager/ResourceManagerMessage_m.h \
  src/wirelessChannel/traceChannel/TraceChannel.h \
  src/helpStructures/DebugInfoWriter.h \
  src/CastaliaMessages.h
