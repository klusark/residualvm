# Android specific build targets

# These must be incremented for each market upload
ANDROID_VERSIONCODE = 1000

ANDROID_TARGET_VERSION = 15

NDK_BUILD = $(ANDROID_NDK)/ndk-build
SDK_ANDROID = $(ANDROID_SDK)/tools/android

PATH_DIST = $(srcdir)/dists/android
PATH_RESOURCES = $(PATH_DIST)/res

PORT_DISTFILES = $(PATH_DIST)/README.Android
DIST_ANDROID_CONTROLS = $(PATH_DIST)/assets/arrows.tga
DIST_JAVA_SRC_DIRS = \
	$(srcdir)/backends/platform/android/org

RESOURCES = \
	$(PATH_BUILD_RES)/values/strings.xml \
	$(PATH_BUILD_RES)/menu/game_menu.xml \
	$(PATH_BUILD_RES)/layout/main.xml \
	$(PATH_BUILD_RES)/layout/splash.xml \
	$(PATH_BUILD_RES)/drawable/gradient.xml \
	$(PATH_BUILD_RES)/drawable/residualvm.png \
	$(PATH_BUILD_RES)/drawable/residualvm_big.png

JAVA_EXTRA_LIBS = \
	$(PATH_BUILD)/libs/ouya-sdk.jar

DIST_ANDROID_MK = $(PATH_DIST)/jni/Android.mk

PATH_BUILD = ./build.tmp
PATH_BUILD_ASSETS = $(PATH_BUILD)/assets
PATH_BUILD_RES = $(PATH_BUILD)/res
PATH_BUILD_SRC = $(PATH_BUILD)/src
PATH_BUILD_LIBRESIDUALVM = $(PATH_BUILD)/libs/armeabi/libresidualvm.so

PATH_GEN_TOP = $(PATH_BUILD)/java
PATH_GEN = $(PATH_GEN_TOP)/$(PATH_REL)
PATH_CLASSES_MAIN = $(PATH_BUILD_CLASSES_MAIN_TOP)/$(PATH_REL)
PATH_CLASSES_PLUGIN = $(PATH_BUILD_CLASSES_PLUGIN_TOP)/$(PATH_REL)

FILE_MANIFEST_SRC = $(srcdir)/dists/android/AndroidManifest.xml
FILE_MANIFEST = $(PATH_BUILD)/AndroidManifest.xml

APK_MAIN = residualvm.apk
APK_PLUGINS = $(patsubst plugins/lib%.so, residualvm-engine-%.apk, $(PLUGINS))

$(FILE_MANIFEST): $(FILE_MANIFEST_SRC)
	@$(MKDIR) -p $(@D)
	sed "s/@ANDROID_VERSIONCODE@/$(ANDROID_VERSIONCODE)/" < $< > $@

$(PATH_BUILD)/res/%: $(PATH_DIST)/res/%
	@$(MKDIR) -p $(@D)
	$(CP) $< $@

$(PATH_BUILD)/src/%: $(PATH_DIST)/src/%
	@$(MKDIR) -p $(@D)
	$(CP) $< $@

$(PATH_BUILD)/libs/%: $(PATH_DIST)/libs/%
	@$(MKDIR) -p $(@D)
	$(CP) $< $@

$(PATH_BUILD_ASSETS): $(DIST_FILES_THEMES) $(DIST_FILES_ENGINEDATA) $(DIST_FILES_SHADERS) $(DIST_ANDROID_CONTROLS)
	$(INSTALL) -d $(PATH_BUILD_ASSETS)
	$(INSTALL) -c -m 644 $(DIST_FILES_THEMES) $(DIST_FILES_ENGINEDATA)  $(DIST_ANDROID_CONTROLS) $(PATH_BUILD_ASSETS)/
ifdef USE_OPENGL_SHADERS
	$(INSTALL) -d $(PATH_BUILD_ASSETS)/shaders
	$(INSTALL) -c -m 644 $(DIST_FILES_SHADERS) $(PATH_BUILD_ASSETS)/shaders
endif

$(PATH_BUILD): $(DIST_ANDROID_MK)
	$(MKDIR) -p $(PATH_BUILD) $(PATH_BUILD)/res
	$(MKDIR) -p $(PATH_BUILD)/src $(PATH_BUILD)/libs $(PATH_BUILD)/jni
	$(CP) $(DIST_ANDROID_MK) $(PATH_BUILD)/jni

$(PATH_BUILD_LIBRESIDUALVM): libresidualvm.so
	$(INSTALL) -c -m 644 libresidualvm.so $(PATH_BUILD)
	$(STRIP) $(PATH_BUILD)/libresidualvm.so
	cd $(PATH_BUILD); $(NDK_BUILD)

$(PATH_BUILD_RES): $(RESOURCES)

$(PATH_BUILD_SRC): $(PATH_BUILD)
	$(MKDIR) $(PATH_BUILD_SRC)
	ln -s $(abspath $(DIST_JAVA_SRC_DIRS)) $(PATH_BUILD_SRC)

$(APK_MAIN): $(PATH_BUILD) $(FILE_MANIFEST) $(PATH_BUILD_RES) $(PATH_BUILD_ASSETS) $(PATH_BUILD_SRC) $(JAVA_EXTRA_LIBS) $(PATH_BUILD_LIBRESIDUALVM)
	$(SDK_ANDROID) update project -p $(PATH_BUILD) -t android-$(ANDROID_TARGET_VERSION) -n ResidualVM
	(cd $(PATH_BUILD); ant debug)
	$(CP) $(PATH_BUILD)/bin/ResidualVM-debug.apk $@


all: $(APK_MAIN)

clean: androidclean

androidclean:
	@$(RM) -rf $(PATH_BUILD) *.apk release

androidrelease: $(addprefix release/, $(APK_MAIN) $(APK_PLUGINS))

androidtestmain: $(APK_MAIN)
	$(ADB) install -r $(APK_MAIN)
	$(ADB) shell am start -a android.intent.action.MAIN -c android.intent.category.LAUNCHER -n org.residualvm.residualvm/.Unpacker

androidtest: $(APK_MAIN) $(APK_PLUGINS)
	@set -e; for apk in $^; do \
		$(ADB) install -r $$apk; \
	done
	$(ADB) shell am start -a android.intent.action.MAIN -c android.intent.category.LAUNCHER -n org.residualvm.residualvm/.Unpacker

# used by buildbot!
androiddistdebug: all
	$(MKDIR) debug
	$(CP) $(APK_MAIN) $(APK_PLUGINS) debug/
	for i in $(DIST_FILES_DOCS) $(PORT_DISTFILES); do \
		sed 's/$$/\r/' < $$i > debug/`basename $$i`.txt; \
	done

.PHONY: androidrelease androidtest $(PATH_BUILD_SRC)
