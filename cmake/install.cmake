# ------------------------------------------------------------------------------
# install.cmake
#
# FHS-compliant installation rules used to build distribution packages
# (e.g. Debian .deb via dpkg-buildpackage / debhelper).
#
# Layout produced (prefix defaults to /usr):
#   <bindir>/NumbatUI                         the GUI binary
#   <bindir>/numbatui                         symlink -> NumbatUI
#   <bindir>/numbatui_indexer                 the indexer binary
#   <datadir>/numbatui/data/...               runtime data (color schemes, gui, ...)
#   <datadir>/numbatui/user/...               user template tree (projects, settings)
#   <datadir>/applications/numbatui.desktop   desktop entry
#   <datadir>/mime/packages/numbatui.xml      MIME association for *.srctrlprj
#   <datadir>/icons/hicolor/scalable/...      scalable application icon
#   <datadir>/icons/hicolor/<size>/...        rasterized icons (if ImageMagick found)
#
# This module is only meaningful on Linux; macOS uses the bundle logic and
# Windows uses the WiX/installer logic elsewhere in the tree.
# ------------------------------------------------------------------------------

if (NOT (UNIX AND NOT APPLE))
	return()
endif()

include(GNUInstallDirs)

# Where NumbatUI's own shared data lives, e.g. /usr/share/numbatui
set(NUMBATUI_PKGDATADIR "${CMAKE_INSTALL_DATADIR}/numbatui")

# --- Binaries -----------------------------------------------------------------
# ${APP_PROJECT_NAME}  -> output name "NumbatUI"
# ${APP_INDEXER_NAME}  -> output name "numbatui_indexer" (set via OUTPUT_NAME)
install(TARGETS ${APP_PROJECT_NAME} ${APP_INDEXER_NAME}
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Lower-case convenience symlink (the .desktop entry launches "numbatui").
# Created relative so it stays valid regardless of the install prefix / DESTDIR.
install(CODE "
	set(_bindir \"\$ENV{DESTDIR}${CMAKE_INSTALL_FULL_BINDIR}\")
	file(MAKE_DIRECTORY \"\${_bindir}\")
	execute_process(COMMAND \"${CMAKE_COMMAND}\" -E create_symlink
		NumbatUI \"\${_bindir}/numbatui\")
")

# --- Runtime data -------------------------------------------------------------
# The application resolves resources at <sharedDataDir>/data/... . On an FHS
# install <sharedDataDir> is <prefix>/share/numbatui (see includesLinux.h).
install(DIRECTORY "${CMAKE_SOURCE_DIR}/bin/app/data/"
	DESTINATION "${NUMBATUI_PKGDATADIR}/data"
	PATTERN "*.bat" EXCLUDE          # Windows-only uninstaller scripts
)

# User template tree (sample projects + ApplicationSettings template). At first
# launch the app copies <sharedDataDir>/user/ into ~/.config/numbatui/.
install(DIRECTORY "${CMAKE_SOURCE_DIR}/bin/app/user/"
	DESTINATION "${NUMBATUI_PKGDATADIR}/user"
)

# --- Desktop integration ------------------------------------------------------
install(FILES "${CMAKE_SOURCE_DIR}/setup/Linux/data/numbatui.desktop"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
)

# shared-mime-info expects the file named after the package.
install(FILES "${CMAKE_SOURCE_DIR}/setup/Linux/data/numbatui-mime.xml"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/mime/packages"
	RENAME numbatui.xml
)

# --- Icons --------------------------------------------------------------------
# Always ship the scalable SVG master.
install(FILES "${CMAKE_SOURCE_DIR}/logo/numbat_ui.svg"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps"
	RENAME numbatui.svg
)

# Additionally rasterize standard hicolor sizes when ImageMagick is available so
# the icon shows up in environments that do not render SVG theme icons. This is
# wired into the build graph (a dependency of the GUI target) rather than done
# at install time, so the generated files exist before `cmake --install`.
find_program(IMAGEMAGICK_CONVERT NAMES magick convert)
if (IMAGEMAGICK_CONVERT)
	set(_icon_master "${CMAKE_SOURCE_DIR}/logo/numbatui_1024.png")
	set(_icon_sizes 16 32 48 64 128 256 512)
	set(_icon_outputs "")
	foreach (_sz IN LISTS _icon_sizes)
		set(_out "${CMAKE_BINARY_DIR}/icons/${_sz}x${_sz}/numbatui.png")
		add_custom_command(
			OUTPUT "${_out}"
			COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/icons/${_sz}x${_sz}"
			COMMAND "${IMAGEMAGICK_CONVERT}" "${_icon_master}"
			        -resize "${_sz}x${_sz}" "${_out}"
			DEPENDS "${_icon_master}"
			COMMENT "Generating ${_sz}x${_sz} application icon"
			VERBATIM
		)
		list(APPEND _icon_outputs "${_out}")
		install(FILES "${_out}"
			DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/${_sz}x${_sz}/apps"
		)
	endforeach()
	add_custom_target(numbatui_icons ALL DEPENDS ${_icon_outputs})
	if (TARGET ${APP_PROJECT_NAME})
		add_dependencies(${APP_PROJECT_NAME} numbatui_icons)
	endif()
else()
	message(STATUS "ImageMagick not found: only the scalable SVG icon will be installed.")
endif()

message(STATUS "Install rules enabled (prefix: ${CMAKE_INSTALL_PREFIX}, data: ${NUMBATUI_PKGDATADIR})")
