#!/bin/bash

################################################################################
#   src/common/post-build.bash
#
#   $1  : target identifier ( Debug, devel, ... )
#   $2  : wx version ( 2.8, 3.0 )
#
################################################################################
TargetId="${1}"
WxVersion="${2}"
echo "> target id  [${TargetId}]"
echo "> wx version [${WxVersion}]"
#   ----------------------------------------------------------------------------
OflpInstallDir=""

if [[ "${WxVersion}" = "3.0" ]] ; then

    OflpInstallDir="./plugin/${TargetId}/linux/wx-3.0.2"

fi

if [[ "${WxVersion}" = "2.8" ]] ; then

    OflpInstallDir="./plugin/${TargetId}/linux/wx-2.8.12"

fi

echo "> installation directory [${OflpInstallDir}]"
#   ----------------------------------------------------------------------------
mkdir -p                                                "${OflpInstallDir}"

cp  "./plugin/${TargetId}/linux/OpenFilesListPlus.so"   "${OflpInstallDir}"

rm -f  "./plugin/${TargetId}/linux/OpenFilesListPlus.so"
rm -f "${OflpInstallDir}/OpenFilesListPlus.zip"
rm -f "${OflpInstallDir}/OpenFilesListPlus.cbplugin"

zip -j9 "${OflpInstallDir}/OpenFilesListPlus.zip"       src/manifest.xml src/oflp.xrc graphics/oflp-panel-header-button-bar-padding.png graphics/oflp-panel-header-button-bar-blue.png graphics/oflp-panel-header-button-bar-green.png graphics/oflp-panel-header-button-bar-orange.png graphics/oflp-panel-header-button-bar-red.png  graphics/oflp-panel-header-button-bar-up.png graphics/oflp-panel-header-button-bar-down.png
zip -j9 "${OflpInstallDir}/OpenFilesListPlus.cbplugin"  "${OflpInstallDir}/OpenFilesListPlus.so" "${OflpInstallDir}/OpenFilesListPlus.zip"

#   overwrite "standalone" for debugging
echo "> updating plugin in [/usr/local/gwr/codeblocks/standalone/svn10692/wx-3.0.2]"
cp -f "${OflpInstallDir}/OpenFilesListPlus.zip" "/usr/local/gwr/codeblocks/standalone/svn10692/wx-3.0.2/codeblocks"
cp -f "${OflpInstallDir}/OpenFilesListPlus.so"  "/usr/local/gwr/codeblocks/standalone/svn10692/wx-3.0.2/lib/codeblocks/plugins"
