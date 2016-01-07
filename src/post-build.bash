#!/bin/bash

WxVersion=$( wx-config --release )
OflpInstallDir=""

if [[ "${WxVersion}" = "3.0" ]] ; then

    echo "> wxWidgets 3.0"
    OflpInstallDir="./plugin/devel/linux/wx-3.0.2"

fi

if [[ "${WxVersion}" = "2.8" ]] ; then

    echo "> wxWidgets 2.8"
    OflpInstallDir="./plugin/devel/linux/wx-2.8.12"

fi

mkdir -p                                            "${OflpInstallDir}"

cp  ./plugin/devel/linux/libOpenFilesListPlus.so    "${OflpInstallDir}"
cp  ./plugin/devel/linux/libOpenFilesListPlus.so    "${OflpInstallDir}/OpenFilesListPlus.so"

rm -f "${OflpInstallDir}/OpenFilesListPlus.zip"
rm -f "${OflpInstallDir}/OpenFilesListPlus.cbplugin"

zip -j9 "${OflpInstallDir}/OpenFilesListPlus.zip"       src/manifest.xml src/oflp.xrc graphics/oflp-panel-header-button-bar-padding.png graphics/oflp-panel-header-button-bar-blue.png graphics/oflp-panel-header-button-bar-green.png graphics/oflp-panel-header-button-bar-orange.png graphics/oflp-panel-header-button-bar-red.png  graphics/oflp-panel-header-button-bar-up.png graphics/oflp-panel-header-button-bar-down.png
zip -j9 "${OflpInstallDir}/OpenFilesListPlus.cbplugin"  "${OflpInstallDir}/OpenFilesListPlus.so" "${OflpInstallDir}/OpenFilesListPlus.zip"
