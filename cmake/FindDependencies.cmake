FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
	MESSAGE(STATUS "Looking for Zlib: found")
ENDIF()

FIND_PACKAGE(Iconv REQUIRED)
IF(Iconv_FOUND)
	MESSAGE(STATUS "Looking for iconv: found")
ENDIF()

IF(USE_OPENAL)
INCLUDE(FindOpenAL)
IF(OPENAL_FOUND)
	MESSAGE(STATUS "Looking for OpenAL: found")
ELSE()
	MESSAGE(WARNING "Looking for OpenAL: not found!")
	MESSAGE(WARNING "If you want to build the OpenAL plugin, get OpenAL from www.openal.org.")
	MESSAGE(WARNING "If it just wasn't found, try setting the OPENALDIR environment variable.")
ENDIF()
ENDIF()

IF(USE_LIBVLC)
	FIND_PACKAGE(LIBVLC MODULE)
	IF(LIBVLC_FOUND)
		MESSAGE(STATUS "Looking for VLC: found")
	ELSE()
		MESSAGE(WARNING "Looking for VLC: not found!")
		MESSAGE(WARNING "If you want to build the VLC plugin, install VLC first.")
	ENDIF()
ENDIF()

IF(USE_FREETYPE)
	INCLUDE(FindFreetype)
	IF(FREETYPE_FOUND)
		MESSAGE(STATUS "Looking for Freetype: found")
	ELSE()
		MESSAGE(WARNING "Looking for Freetype: not found!")
		MESSAGE(WARNING "If you want to build the TTF plugin, install Freetype first.")
		MESSAGE(WARNING "It is required for our demo to work!")
	ENDIF()
ENDIF()

IF(USE_PNG)
	IF(APPLE AND DEFINED ENV{GITHUB_SHA})
		INCLUDE(DeduceLibpngPath)
		DEDUCE_LIBPNG_PATH()

		if(NOT LIBPNG_PATH)
			message(FATAL_ERROR "Cannot deduce LIBPNG_PATH!")
		endif()

		SET(PNG_INCLUDE_DIRS "${LIBPNG_PATH}/include")
		SET(PNG_LIBRARIES "${LIBPNG_PATH}/lib/libpng16.16.dylib")
		SET(PNG_FOUND TRUE)
	ELSE()
		INCLUDE(FindPNG)
	ENDIF()
	IF(PNG_FOUND)
		MESSAGE(STATUS "Looking for libPNG: found")
	ELSE()
		MESSAGE(WARNING "Looking for libPNG: not found!")
		MESSAGE(WARNING "GemRB will be built without any PNG support. Get it from www.libpng.org" )
		MESSAGE(WARNING "While no original game data is in PNG format, some mod data is and will need conversion.")
		MESSAGE(WARNING "It is required for our demo to work!")
	ENDIF()
ENDIF()

IF(USE_VORBIS)
	FIND_LIBRARY(VORBIS_LIBRARY vorbisfile)
	IF(VORBIS_LIBRARY)
		find_path(VORBIS_FILE vorbisfile.h PATH_SUFFIXES vorbis)
		IF(VORBIS_FILE)
			MESSAGE(STATUS "Looking for Ogg Vorbis support: found")
		ELSE()
			unset(VORBIS_LIBRARY) # disable the build for this plugin
		ENDIF()
	ENDIF()
	IF(NOT VORBIS_LIBRARY)
		MESSAGE(WARNING "Looking for Ogg Vorbis support: not found!")
		MESSAGE(WARNING "While no original game data is in OGG format, some mod data is and will need conversion.")
		MESSAGE(WARNING "It is required for our demo to work!")
	ENDIF()
ENDIF()