#makefile for PDF editor
.PHONY: distclean clean pdfedit5 kernel kpdf-kde-3.3.2 utils qoutputdevices qsa qsa_configure qsa4 qsa4_configure gui

# includes basic building rules
# REL_ADDR has to be defined, because Makefile.rules refers 
# to the Makefile.flags
REL_ADDR=../
include $(REL_ADDR)Makefile.rules

# make PDF editor - source and documentation
# targets are determined during configure
all: kernel tests $(GUI_TARGET) $(TOOLS_TARGET)

# PDF editor
pdfedit5: utils kernel  qoutputdevices qsa $(GUIROOT)/Makefile.qt
	cd $(GUIROOT) && $(MAKE) -f Makefile.gui

# PDFedit core development package for 3rd party code
pdfedit5-core-dev:

	cd $(UTILSROOT) && $(MAKE) staticlib
	cd $(KERNELROOT) && $(MAKE) staticlib

# generates QT makefile for gui
# This monkey stuff is here because i was not able to persuade qmake
# to include our Makefile.flags which contains all configuration stuff
# Therefore we are building in two stages:
# * gui/Makefile.gui simply generates gui/Makefile from gui/pdfedit.pro
#   and includes this file to the gui/Makefile.gui
# * gui/Makefile.qt is in full power of qmake and we simply refers to our
#   configuration variables. These are not visible to the Makefile 
#   directly, but we never use gui/Makefile.qt directly but rather as
#   included in gui/Makefile.gui
# Seems to be complicated and if someone finds better solution how to
# get include ../../Makefile.flags into the generated makefile then
# it can be safely removed.
# We need to generate online help (unless it is generated yet) before
# QT makefile is created (because of file expansion used in project
# file)
# For the same reason we have to generate localization files too
$(GUIROOT)/Makefile.qt: $(GUIROOT)/Makefile.gui $(GUIROOT)/pdfedit5.pro
	$(SHELL) $(ROOT)/tools/generate_online_help.sh "$(DOCROOT)" "$(GUIROOT)"
	$(SHELL) $(ROOT)/tools/generate_lang.sh "$(GUIROOT)" "$(LRELEASE)"
	cd $(GUIROOT) && $(QMAKE) pdfedit5.pro -o Makefile.qt

# QOutputDevices - uses same trick with Makefile.{gui.qt} like
# gui - see above
qoutputdevices:  $(QOUTPUTDEVROOT)/Makefile.qt
	cd $(QOUTPUTDEVROOT) && $(MAKE) -f Makefile.gui staticlib

$(QOUTPUTDEVROOT)/Makefile.qt: $(QOUTPUTDEVROOT)/Makefile.gui $(QOUTPUTDEVROOT)/qoutputdevices.pro
	cd $(QOUTPUTDEVROOT) && $(QMAKE) -o Makefile.qt

# Utils
utils: 
	cd $(UTILSROOT) && $(MAKE) staticlib

#QSA
qsa: $(QSAROOT)/Makefile.qsa
	cd $(QSAROOT) && $(MAKE) staticlib

$(QSAROOT)/Makefile.qsa: $(QSAROOT)/qsa.pro
	cd $(QSAROOT) && ./configure "${QMAKE}" "${E_RELEASE}"

# Kernel
kernel:  utils
	cd $(KERNELROOT) && $(MAKE) staticlib

# Tools
tools: kernel 
	cd $(TOOLSROOT) && $(MAKE)

# kernel tests
tests: kernel 
	cd $(TESTSROOT) && $(MAKE) 

# internal rule for development which should be used when too many
# changes were introduced
dev-rebuild: dev-clean utils kernel tests pdfedit5

# Cleanup
clean:
	cd $(GUIROOT) && $(MAKE) -f Makefile.gui clean || true
	cd $(QSAROOT) && $(MAKE) clean || true
	cd $(KERNELROOT) && $(MAKE) clean || true
	cd $(TOOLSROOT) && $(MAKE) clean || true
	cd $(TESTSROOT) && $(MAKE) clean || true
	cd $(UTILSROOT) && $(MAKE) clean
	cd $(QOUTPUTDEVROOT) && $(MAKE) -f Makefile.gui clean || true
	$(DEL_FILE) $(GUIROOT)/Makefile.qt 
	$(DEL_FILE) $(QOUTPUTDEVROOT)/Makefile.qt

dev-clean:
	cd $(GUIROOT) && $(MAKE) -f Makefile.gui clean || true
	cd $(KERNELROOT) && $(MAKE) clean || true
	cd $(TOOLSROOT) && $(MAKE) clean || true
	cd $(TESTSROOT) && $(MAKE) clean || true
	cd $(UTILSROOT) && $(MAKE) clean


# Dist Cleanup
distclean: 
	cd $(GUIROOT) && $(MAKE) -f Makefile.gui distclean || true
	cd $(QSAROOT) && $(MAKE) distclean || true
	cd $(KERNELROOT) && $(MAKE) distclean || true
	cd $(TOOLSROOT) && $(MAKE) clean || true
	cd $(TESTSROOT) && $(MAKE) distclean || true
	cd $(UTILSROOT) && $(MAKE) distclean || true
	cd $(QOUTPUTDEVROOT) && $(MAKE) -f Makefile.gui distclean || true
	$(DEL_FILE) $(QOUTPUTDEVROOT)/Makefile || true
	$(DEL_FILE) $(UTILSROOT)/aconf.h || true
	$(DEL_FILE $(GUIROOT)/config.h $(GUIROOT)/Makefile.qt $(GUIROOT)/menugenerator $(GUIROOT)/pdfedit5 || true
	$(DEL_FILE) $(QOUTPUTDEVROOT)/Makefile.qt
