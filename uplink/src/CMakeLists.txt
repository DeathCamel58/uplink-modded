cmake_minimum_required (VERSION 3.0.0)
project(uplink)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR “In-source builds not allowed! Create a build directory and run CMake from there. ” )
endif()

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

add_definitions(-DFULLGAME=1
        -D_DEBUG=1
        -DENABLE_BINRELOC
        -DDEBUGLOG_ENABLED)

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