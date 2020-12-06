# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(	src/main/cpp/Game/Engine )
include_directories(	src/main/cpp/Game/Base )
include_directories(	src/main/cpp/Game/Component	)
include_directories(	src/main/cpp/Game/Entity	)
include_directories(	src/main/cpp/Game/Game	)
include_directories(	src/main/cpp/Game/Game/Manager	)
include_directories(	src/main/cpp/Game/Scene	)
include_directories(	src/main/cpp/Game/Boot	)
include_directories(	src/main/cpp/Game/App/Param		)
include_directories(	src/main/cpp/Game/App/Entity	)
include_directories(	src/main/cpp/Game/App/Entity/Manager	)

add_library(	jni_main-lib SHARED src/main/cpp/jni_main.cpp )

add_library(    aoiyume_pch-lib SHARED src/main/cpp/Game/aoiyume_pch.cpp )

add_library(    math-lib SHARED
				src/main/cpp/Math/math_Vector.cpp
				src/main/cpp/Math/math_Matrix.cpp  )

add_library(	aoiyume_engine-lib SHARED
				src/main/cpp/Game/Engine/engine-lib.cpp
				src/main/cpp/Game/Engine/Engine.cpp	)

add_library(	base-lib SHARED
				src/main/cpp/Game/Base/ComponentBase.cpp
                src/main/cpp/Game/Base/EntityBase.cpp
                src/main/cpp/Game/Base/SceneBase.cpp
                src/main/cpp/Game/Base/SceneManager.cpp
				src/main/cpp/Game/Base/Singleton.cpp)

add_library(	component-lib SHARED
				src/main/cpp/Game/Component/DrawComponent.cpp
				src/main/cpp/Game/Component/TransformComponent.cpp
				src/main/cpp/Game/Component/CollisionComponent.cpp
				src/main/cpp/Game/Component/CameraComponent.cpp
				src/main/cpp/Game/Component/LayoutComponent.cpp
				src/main/cpp/Game/Component/CollisionManager.cpp	)

add_library(	entity-lib	SHARED
				src/main/cpp/Game/Entity/Entity.cpp
				src/main/cpp/Game/Entity/EntityManager.cpp )

add_library(	game-lib SHARED
				src/main/cpp/Game/Game/GameEntity.cpp
				src/main/cpp/Game/Game/Button/ButtonBase.cpp
				src/main/cpp/Game/Game/Button/DebugButton.cpp
				src/main/cpp/Game/Game/Button/SimpleButton.cpp
				src/main/cpp/Game/Game/Button/SimpleCursor.cpp
		        src/main/cpp/Game/Game/Button//ButtonManager.cpp
				src/main/cpp/Game/Game/MessageWindow/MessageWindow.cpp)


add_library(	app_entity-lib SHARED
				src/main/cpp/Game/App/Entity/Character.cpp
				src/main/cpp/Game/App/Entity/InformationPlate.cpp
				src/main/cpp/Game/App/Entity/Dice.cpp)

add_library(	app_scene-lib SHARED
				src/main/cpp/Game/App/Scene/SceneTitle.cpp
				src/main/cpp/Game/App/Scene/SceneGameMain.cpp
				src/main/cpp/Game/App/Scene/SceneAdv.cpp
				src/main/cpp/Game/App/Scene/SceneSpendTime.cpp
				src/main/cpp/Game/App/Scene/SceneWork.cpp
				src/main/cpp/Game/App/Scene/SceneShop.cpp)

add_library(	app_param-lib SHARED
				src/main/cpp/Game/App/Param/AppParam.cpp)

add_library(	boot-lib SHARED
				src/main/cpp/Game/Boot/gamemain-lib.cpp
				src/main/cpp/Game/Boot/SceneLauncher.cpp	)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# Sets the name of the path variable.
# Specifies the name of the NDK library that
# you want CMake to locate.
find_library(
                log-lib log
)

find_library(
				opengl-lib GLESv2
)

find_library(
				android-lib android
)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# Specifies the target library.
# Links the target library to the log library
# included in the NDK.

target_link_libraries( aoiyume_pch-lib ${log-lib} ${android-lib} )
target_link_libraries( math-lib aoiyume_pch-lib )
target_link_libraries( aoiyume_engine-lib aoiyume_pch-lib ${opengl-lib} )
target_link_libraries( jni_main-lib aoiyume_engine-lib )

target_link_libraries( base-lib math-lib aoiyume_engine-lib )
target_link_libraries( component-lib base-lib)
target_link_libraries( entity-lib component-lib )
target_link_libraries( game-lib entity-lib)
target_link_libraries( app_param-lib base-lib )
target_link_libraries( app_entity-lib game-lib app_param-lib)
target_link_libraries( app_scene-lib app_entity-lib app_param-lib )
target_link_libraries( boot-lib app_scene-lib )