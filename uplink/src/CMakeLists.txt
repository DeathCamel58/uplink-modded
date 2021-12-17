cmake_minimum_required (VERSION 3.0.0)
project(uplink C CXX)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

# Set game type and add definition
set(UPLINK_BUILDGAMETYPE "FULLGAME" CACHE STRING "Type of game build to target")
set_property(CACHE UPLINK_BUILDGAMETYPE PROPERTY STRINGS FULLGAME TESTGAME DEMOGAME)
message("UPLINK_BUILDGAMETYPE: '${UPLINK_BUILDGAMETYPE}'")
add_definitions(-D${UPLINK_BUILDGAMETYPE})

# Set version of uplink
SET(UPLINK_VERSION "10.0" CACHE STRING "Type of game build to target")
message("UPLINK_VERSION: '${UPLINK_VERSION}'")
SET(UPLINK_VERSION_NAME "RELEASE" CACHE STRING "Name/status of release")
message("UPLINK_VERSION_NAME: '${UPLINK_VERSION_NAME}'")

# Set current save file version and minimum save file version
SET(UPLINK_SAVEFILE_VERSION "SAV62" CACHE STRING "Max version is SAVZZ (due to the number of characters to read)")
message("UPLINK_SAVEFILE_VERSION: '${UPLINK_SAVEFILE_VERSION}'")
SET(UPLINK_SAVEFILE_VERSION_MIN "SAV56" CACHE STRING "Minimun Savefile version to run Uplink")
message("UPLINK_SAVEFILE_VERSION_MIN: '${UPLINK_SAVEFILE_VERSION_MIN}'")

# Use SDL instead of GLUT
option(UPLINK_USE_SDL "Use SDL instead of glut" OFF)
if (UPLINK_USE_SDL)
    add_definitions(-DUSE_SDL)
endif ()
message("UPLINK_USE_SDL: '${UPLINK_USE_SDL}'")

# Set the alpha value for interface
SET(UPLINK_ALPHA "0.85f" CACHE STRING "Alpha value used for interface")
message("UPLINK_ALPHA: '${UPLINK_ALPHA}'")
SET(UPLINK_ALPHA_DISABLED "0.5" CACHE STRING "Alpha value for disabled buttons")
message("UPLINK_ALPHA_DISABLED: '${UPLINK_ALPHA_DISABLED}'")

# This version designed for DOC labs
option(UPLINK_DOCLABRELEASE "This version designed for DOC labs" OFF)
if (UPLINK_DOCLABRELEASE)
    add_definitions(-DDOCLABRELEASE)
endif ()
message("UPLINK_DOCLABRELEASE: '${UPLINK_DOCLABRELEASE}'")

# Set if this is a warez release
option(UPLINK_WAREZRELEASE "Should this release ignore license status" ON)
if (UPLINK_WAREZRELEASE)
    add_definitions(-DWAREZRELEASE)
endif ()

# Eg all-links, all-software, password-bypass etc
option(UPLINK_CHEATMODES_ENABLED "Eg all-links, all-software, password-bypass etc" OFF)
if (UPLINK_CHEATMODES_ENABLED)
    add_definitions(-DCHEATMODES_ENABLED)
endif ()
message("UPLINK_CHEATMODES_ENABLED: '${UPLINK_CHEATMODES_ENABLED}'")

# Do the code card lookup thingy at the start
option(UPLINK_CODECARD_ENABLED "Do the code card lookup thingy at the start" OFF)
if (UPLINK_CODECARD_ENABLED)
    add_definitions(-DCODECARD_ENABLED)
endif ()
message("UPLINK_CODECARD_ENABLED: '${UPLINK_CODECARD_ENABLED}'")

# Redirect std* to users/debug.log
option(UPLINK_DEBUGLOG_ENABLED "Redirect std* to users/debug.log" OFF)
if (UPLINK_DEBUGLOG_ENABLED)
    add_definitions(-DDEBUGLOG_ENABLED)
endif ()
message("UPLINK_DEBUGLOG_ENABLED: '${UPLINK_DEBUGLOG_ENABLED}'")

# Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies
option(UPLINK_CODECARD_CHRIS "Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies" ON)
if (UPLINK_CODECARD_CHRIS)
    add_definitions(-DCODECARD_CHRIS)
endif ()
message("UPLINK_CODECARD_CHRIS: '${UPLINK_CODECARD_CHRIS}'")

# Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies
option(UPLINK_CODECARD_TOM "Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies" ON)
if (UPLINK_CODECARD_TOM)
    add_definitions(-DCODECARD_TOM)
endif ()
message("UPLINK_CODECARD_TOM: '${UPLINK_CODECARD_TOM}'")

# Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies
option(UPLINK_CODECARD_MARK "Which code card to use. After V1.0 we use ALL THREE - to allow a unified patch that works on all copies" ON)
if (UPLINK_CODECARD_MARK)
    add_definitions(-DCODECARD_MARK)
endif ()
message("UPLINK_CODECARD_MARK: '${UPLINK_CODECARD_MARK}'")

# TODO: Find out what this does and fill in the description
option(UPLINK_BETA_TIMEOUT "" OFF)
if (UPLINK_BETA_TIMEOUT)
    add_definitions(-DBETA_TIMEOUT)
endif ()
message("UPLINK_BETA_TIMEOUT: '${UPLINK_BETA_TIMEOUT}'")

# Show the winning code on the obituary interface and send a ingame message when the player win the game
option(UPLINK_GAME_WINNING_CODE "Show the winning code on the obituary interface and send a ingame message when the player win the game" OFF)
if (UPLINK_GAME_WINNING_CODE)
    add_definitions(-DGAME_WINNING_CODE)
endif ()
message("UPLINK_GAME_WINNING_CODE: '${UPLINK_GAME_WINNING_CODE}'")

# Is the multi-monitor support is enabled
option(UPLINK_MULTIMONITOR_SUPPORT "Is the multi-monitor support is enabled" OFF)
if (UPLINK_MULTIMONITOR_SUPPORT)
    add_definitions(-DMULTIMONITOR_SUPPORT)
endif ()
message("UPLINK_MULTIMONITOR_SUPPORT: '${UPLINK_MULTIMONITOR_SUPPORT}'")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/app/globals_defines.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/app/globals_defines.h")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Dependencies for Uplink
include_directories(../lib/eclipse)
include_directories(../lib/gucci)
include_directories(../lib/tosser)
include_directories(../lib/mmgr)
include_directories(../lib/vanbakel)
include_directories(../lib/soundgarden)
include_directories(../lib/redshirt)
include_directories(../lib/bungle)
include_directories(../lib/tcp4u.331/Include)
include_directories(../lib/irclib ../lib/irclib/linux)



add_definitions(-DENABLE_BINRELOC)

add_executable(${PROJECT_NAME}
        app/app.cpp
        app/dos2unix.cpp
        app/miscutils.cpp
        app/opengl.cpp
        app/opengl_interface.cpp
        app/probability.cpp
        app/serialise.cpp
        app/uplinkobject.cpp
        game/data/data.cpp
        game/game.cpp
        game/gameobituary.cpp
        game/scriptlibrary.cpp
        interface/interface.cpp
        interface/interfacescreen.cpp
        interface/localinterface/analyser_interface.cpp
        interface/localinterface/cheat_interface.cpp
        interface/localinterface/email_interface.cpp
        interface/localinterface/evtqueue_interface.cpp
        interface/localinterface/finance_interface.cpp
        interface/localinterface/gateway_interface.cpp
        interface/localinterface/hud_interface.cpp
        interface/localinterface/hw_interface.cpp
        interface/localinterface/irc_interface.cpp
        interface/localinterface/keyboardinterface.cpp
        interface/localinterface/lan_interface.cpp
        interface/localinterface/localinterface.cpp
        interface/localinterface/localinterfacescreen.cpp
        interface/localinterface/memory_interface.cpp
        interface/localinterface/mission_interface.cpp
        interface/localinterface/phonedialler.cpp
        interface/localinterface/sendmail_interface.cpp
        interface/localinterface/status_interface.cpp
        interface/localinterface/sw_interface.cpp
        interface/localinterface/worldmap/worldmap_interface.cpp
        interface/localinterface/worldmap/worldmap_layout.cpp
        interface/remoteinterface/academicscreen_interface.cpp
        interface/remoteinterface/accountscreen_interface.cpp
        interface/remoteinterface/bbsscreen_interface.cpp
        interface/remoteinterface/changegatewayscreen_interface.cpp
        interface/remoteinterface/codecardscreen_interface.cpp
        interface/remoteinterface/companyinfoscreen_interface.cpp
        interface/remoteinterface/consolescreen_interface.cpp
        interface/remoteinterface/contactscreen_interface.cpp
        interface/remoteinterface/criminalscreen_interface.cpp
        interface/remoteinterface/cypherscreen_interface.cpp
        interface/remoteinterface/dialogscreen_interface.cpp
        interface/remoteinterface/disconnectedscreen_interface.cpp
        interface/remoteinterface/faithscreen_interface.cpp
        interface/remoteinterface/fileserverscreen_interface.cpp
        interface/remoteinterface/highsecurityscreen_interface.cpp
        interface/remoteinterface/hwsalesscreen_interface.cpp
        interface/remoteinterface/linksscreen_interface.cpp
        interface/remoteinterface/loansscreen_interface.cpp
        interface/remoteinterface/logscreen_interface.cpp
        interface/remoteinterface/menuscreen_interface.cpp
        interface/remoteinterface/messagescreen_interface.cpp
        interface/remoteinterface/nearestgatewayscreen_interface.cpp
        interface/remoteinterface/newsscreen_interface.cpp
        interface/remoteinterface/nuclearwarscreen_interface.cpp
        interface/remoteinterface/passwordscreen_interface.cpp
        interface/remoteinterface/protovisionscreen_interface.cpp
        interface/remoteinterface/radiotransmitterscreen_interface.cpp
        interface/remoteinterface/rankingscreen_interface.cpp
        interface/remoteinterface/recordscreen_interface.cpp
        interface/remoteinterface/remoteinterface.cpp
        interface/remoteinterface/remoteinterfacescreen.cpp
        interface/remoteinterface/securityscreen_interface.cpp
        interface/remoteinterface/shareslistscreen_interface.cpp
        interface/remoteinterface/sharesviewscreen_interface.cpp
        interface/remoteinterface/socialsecurityscreen_interface.cpp
        interface/remoteinterface/swsalesscreen_interface.cpp
        interface/remoteinterface/useridscreen_interface.cpp
        interface/remoteinterface/voiceanalysisscreen_interface.cpp
        interface/remoteinterface/voicephonescreen_interface.cpp
        interface/scrollbox.cpp
        interface/taskmanager/decrypter.cpp
        interface/taskmanager/decypher.cpp
        interface/taskmanager/defrag.cpp
        interface/taskmanager/dictionaryhacker.cpp
        interface/taskmanager/faith.cpp
        interface/taskmanager/filecopier.cpp
        interface/taskmanager/filedeleter.cpp
        interface/taskmanager/firewalldisable.cpp
        interface/taskmanager/gatewaynuke.cpp
        interface/taskmanager/iplookup.cpp
        interface/taskmanager/ipprobe.cpp
        interface/taskmanager/lanforce.cpp
        interface/taskmanager/lanprobe.cpp
        interface/taskmanager/lanscan.cpp
        interface/taskmanager/lanspoof.cpp
        interface/taskmanager/logdeleter.cpp
        interface/taskmanager/logmodifier.cpp
        interface/taskmanager/logundeleter.cpp
        interface/taskmanager/motionsensor.cpp
        interface/taskmanager/passwordbreaker.cpp
        interface/taskmanager/proxydisable.cpp
        interface/taskmanager/revelation.cpp
        interface/taskmanager/revelationtracker.cpp
        interface/taskmanager/securitybypass.cpp
        interface/taskmanager/taskmanager.cpp
        interface/taskmanager/tracetracker.cpp
        interface/taskmanager/tutorial.cpp
        interface/taskmanager/uplinkagentlist.cpp
        interface/taskmanager/uplinktask.cpp
        interface/taskmanager/voiceanalyser.cpp
        mainmenu/closing_interface.cpp
        mainmenu/connectionlost_interface.cpp
        mainmenu/demogameover_interface.cpp
        mainmenu/disavowed_interface.cpp
        mainmenu/firsttimeloading_interface.cpp
        mainmenu/genericoptions_interface.cpp
        mainmenu/loading_interface.cpp
        mainmenu/login_interface.cpp
        mainmenu/mainmenu.cpp
        mainmenu/mainmenuscreen.cpp
        mainmenu/graphicoptions_interface.cpp
        mainmenu/networkoptions_interface.cpp
        mainmenu/obituary_interface.cpp
        mainmenu/options_interface.cpp
        mainmenu/revelationlost_interface.cpp
        mainmenu/revelationwon_interface.cpp
        mainmenu/theme_interface.cpp
        mainmenu/theteam_interface.cpp
        mainmenu/warezgameover_interface.cpp
        network/clientconnection.cpp
        network/interfaces/clientcommsinterface.cpp
        network/interfaces/clientstatusinterface.cpp
        network/interfaces/networkscreen.cpp
        network/network.cpp
        network/networkclient.cpp
        network/networkserver.cpp
        options/options.cpp
        uplink.cpp
        view/fps.cpp
        view/view.cpp
        world/agent.cpp
        world/company/company.cpp
        world/company/companyuplink.cpp
        world/company/mission.cpp
        world/company/news.cpp
        world/company/sale.cpp
        world/computer/bankaccount.cpp
        world/computer/bankcomputer.cpp
        world/computer/computer.cpp
        world/computer/computerscreen/bbsscreen.cpp
        world/computer/computerscreen/computerscreen.cpp
        world/computer/computerscreen/cypherscreen.cpp
        world/computer/computerscreen/dialogscreen.cpp
        world/computer/computerscreen/disconnectedscreen.cpp
        world/computer/computerscreen/genericscreen.cpp
        world/computer/computerscreen/highsecurityscreen.cpp
        world/computer/computerscreen/linksscreen.cpp
        world/computer/computerscreen/logscreen.cpp
        world/computer/computerscreen/menuscreen.cpp
        world/computer/computerscreen/messagescreen.cpp
        world/computer/computerscreen/passwordscreen.cpp
        world/computer/computerscreen/shareslistscreen.cpp
        world/computer/computerscreen/useridscreen.cpp
        world/computer/databank.cpp
        world/computer/gateway.cpp
        world/computer/gatewaydef.cpp
        world/computer/lancomputer.cpp
        world/computer/lanmonitor.cpp
        world/computer/logbank.cpp
        world/computer/recordbank.cpp
        world/computer/security.cpp
        world/computer/securitymonitor.cpp
        world/computer/securitysystem.cpp
        world/connection.cpp
        world/date.cpp
        world/generator/consequencegenerator.cpp
        world/generator/demoplotgenerator.cpp
        world/generator/langenerator.cpp
        world/generator/missiongenerator.cpp
        world/generator/namegenerator.cpp
        world/generator/newsgenerator.cpp
        world/generator/numbergenerator.cpp
        world/generator/plotgenerator.cpp
        world/generator/recordgenerator.cpp
        world/generator/worldgenerator.cpp
        world/message.cpp
        world/person.cpp
        world/player.cpp
        world/rating.cpp
        world/scheduler/arrestevent.cpp
        world/scheduler/attemptmissionevent.cpp
        world/scheduler/bankrobberyevent.cpp
        world/scheduler/changegatewayevent.cpp
        world/scheduler/eventscheduler.cpp
        world/scheduler/installhardwareevent.cpp
        world/scheduler/notificationevent.cpp
        world/scheduler/runplotsceneevent.cpp
        world/scheduler/seizegatewayevent.cpp
        world/scheduler/shotbyfedsevent.cpp
        world/scheduler/uplinkevent.cpp
        world/scheduler/warningevent.cpp
        world/vlocation.cpp
        world/world.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE eclipse)
target_link_libraries(${PROJECT_NAME} PRIVATE redshirt)
target_link_libraries(${PROJECT_NAME} PRIVATE gucci)
target_link_libraries(${PROJECT_NAME} PRIVATE tiff)
target_link_libraries(${PROJECT_NAME} PRIVATE ftgl)
target_link_libraries(${PROJECT_NAME} PRIVATE irclib)
target_link_libraries(${PROJECT_NAME} PRIVATE tcp4u)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
target_link_libraries(${PROJECT_NAME} PRIVATE soundgarden)
target_link_libraries(${PROJECT_NAME} PRIVATE vanbakel)
target_link_libraries(${PROJECT_NAME} PRIVATE unrar)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer)
target_link_libraries(${PROJECT_NAME} PRIVATE bungle)

add_library(binreloc app/binreloc.cpp app/binreloc.h)
add_definitions(-fpermissive)
target_link_libraries(${PROJECT_NAME} PRIVATE binreloc)

# Copy data files to build directory
add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)
