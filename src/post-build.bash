#!/bin/bash

WxVersion=$( wx-config --release )
OflpInstallDir=""

if [[ "${WxVersion}" = "3.0" ]] ; then

    echo "> wxWidgets 3.0"
    OflpInstallDir="./plugin/wx-3.0.2"

fi

if [[ "${WxVersion}" = "2.8" ]] ; then

    echo "> wxWidgets 2.8"
    OflpInstallDir="./plugin/wx-2.8.12"

fi

zip -j9 "${OflpInstallDir}/OpenFilesListPlus.zip" src/manifest.xml src/oflp.xrc graphics/oflp-panel-header-button-bar-padding.png graphics/oflp-panel-header-button-bar-blue.png graphics/oflp-panel-header-button-bar-green.png graphics/oflp-panel-header-button-bar-orange.png graphics/oflp-panel-header-button-bar-red.png  graphics/oflp-panel-header-button-bar-up.png graphics/oflp-panel-header-button-bar-down.png
cp  ./plugin/libOpenFilesListPlus.so "${OflpInstallDir}"
#zip -j9 plugin/linux_x86x64_wx-3.0.2/OpenFilesListPlus.cbplugin plugin/linux_x86x64_wx-3.0.2/libOpenFilesListPlus.so plugin/linux_x86x64_wx-3.0.2/OpenFilesListPlus.zip
