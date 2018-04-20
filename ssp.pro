TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = ssp_d
} else {
    TARGET = ssp
}

DESTDIR = $$PWD/build/bin/
MOC_DIR = $$PWD/build/moc/
OBJECTS_DIR = $$PWD/build/obj/

SOURCES += \
    src/EventReceiver/EventReceiver.cpp \
    src/Config/Ini.cpp \
    src/Config/Inireader.cpp \
    src/Config/Config.cpp \
    src/Player/Player.cpp \
    src/Player/FMS.cpp \
    src/Player/ShootIntersection.cpp \
    src/Player/PlayerAction.cpp \
#    src/RealisticWater.cpp \
#    src/childsquarenode.cpp \
    src/Common/Collision.cpp \
#    src/abstractscenenode.cpp \
#    src/fountainsquarenode.cpp \
    src/Common/Common.cpp \
    src/Common/SignalSlot.cpp \
#    src/attractionsquarenode.cpp \
#    src/grass/TKGrassSceneNode.cpp \
#    src/churchscenenode.cpp \
#    src/hotelscenenode.cpp \
    src/Monster/AI.cpp \
    src/Monster/MonsterNode.cpp \
#    src/MyBlur.cpp \
#    src/CGrassSurfaceSceneNode.cpp \
#    src/AircraftSceneNode.cpp \
#    src/LakeSceneNode.cpp \
    src/UI/SceneMode.cpp \
    src/SceneNode/AtomicBoom.cpp \
    src/UI/MessageBox.cpp \
    src/UI/InOutFader.cpp \
    src/UI/StaticText.cpp \
    src/Weapon/Weapon.cpp \
    src/Weapon/AnimationFrameLoop.cpp \
    src/Weapon/AK.cpp \
    src/Weapon/WeaponAction.cpp \
    src/Weapon/CZ805.cpp \
    src/Weapon/AnimationEndCallback.cpp \
#  src/Decal/CBatchingMesh.cpp \
#  src/Decal/DecalManager.cpp \
#  src/Decal/DecalSceneNode.cpp \
    src/SoundEngine/SoundEngine.cpp \
    src/main.cpp \
    src/GamePark.cpp \
    src/SceneNode/LadderSceneNode.cpp \
    src/SceneNode/MotionPictureCredits.cpp \
    src/Weapon/Knife.cpp \
    src/Monster/MonsterFMS.cpp \
    src/Monster/RespawnPoint.cpp

HEADERS += \
    src/EventReceiver/EventReceiver.h \
    src/Config/Ini.h \
    src/Config/Inireader.h \
    src/Config/Config.h \
    src/Player/FMS.h \
    src/Player/ShootIntersection.h \
    src/Player/Player.h \
    src/Player/PlayerAction.h \
#    src/RealisticWater.h \
#    src/postprocessmotionblur.h \
#    src/childsquarenode.h \
    src/Common/Collision.h \
#    src/abstractscenenode.h \
#    src/fountainsquarenode.h \
    src/Common/Common.h \
    src/Common/SignalSlot.h \
#    src/attractionsquarenode.h \
#    src/grass/TKGrassSceneNode.h \
#    src/grass/TKGrassSceneNode_ShaderCB.h \
#    src/churchscenenode.h \
#    src/hotelscenenode.h \
    src/Monster/AI.h \
    src/Monster/MonsterNode.h \
#    src/MyBlur.h \
#    src/CGrassSurfaceSceneNode.h \
#    src/AircraftSceneNode.h \
#    src/LakeSceneNode.h \
    src/UI/SceneMode.h \
    src/SceneNode/AtomicBoom.h \
    src/UI/MessageBox.h \
    src/UI/InOutFader.h \
    src/UI/StaticText.h \
    src/Weapon/Weapon.h \
    src/Weapon/AnimationFrameLoop.h \
    src/Weapon/AK.h \
    src/Weapon/WeaponAction.h \
    src/Weapon/CZ805.h \
    src/Weapon/AnimationEndCallback.h \
#  src/Decal/CBatchingMesh.h \
#  src/Decal/DecalManager.h \
#  src/Decal/DecalSceneNode.h \
    src/SoundEngine/SoundEngine.h \
    src/GamePark.h \
    src/SceneNode/LadderSceneNode.h \
    src/SceneNode/MotionPictureCredits.h \
    src/Weapon/Knife.h \
    src/Monster/MonsterFMS.h \
    src/Monster/RespawnPoint.h

LIBS += -L../../irrlicht-1.8.4/lib/Linux/ -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11
LIBS += -L../../irrKlang-64bit-1.5.0/bin/linux-gcc-64/ -lIrrKlang
#LIBS += -L../../irrlicht-1.7.3/lib/Linux/ -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11


INCLUDEPATH += ../../irrlicht-1.8.4/include
INCLUDEPATH += ../../irrKlang-64bit-1.5.0/include
#INCLUDEPATH += ../../irrlicht-1.7.3/include
INCLUDEPATH += src

OTHER_FILES += $$PWD/config/config.ini

