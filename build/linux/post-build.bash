#!/bin/bash

####################################################################################################
#   src/common/post-build.bash
#
#   $1  : target identifier ( Debug, devel, ... )
#   $2  OFLP_WXVERSION
#   $3  OFLP_WXTOOLKIT
#
####################################################################################################
TargetId="${1}"
OflpWxVersion="${2}"
OflpWxToolkit="${3}"

echo "> Options"
echo "  target id  [${TargetId}]"
echo "  wx version [${OflpWxVersion}]"
echo "  wx toolkit [${OflpWxToolkit}]"
#   ------------------------------------------------------------------------------------------------
OflpInstallDir=""

if [[ "${OflpWxVersion}" = "3.0" ]] ; then

    OflpWxVersion="3.0.4"

fi

if [[ "${OflpWxVersion}" = "2.8" ]] ; then

    OflpWxVersion="2.8.12"

fi

OflpInstallDir="./plugin/${TargetId}/linux/wx-${OflpWxVersion}"
echo "  plugin       directory [${OflpInstallDir}]"

OflpSystemDir="/usr/local/gwr/codeblocks/standalone/svn11267/wx-${OflpWxVersion}"
echo "  installation directory [${OflpSystemDir}]"
#   ------------------------------------------------------------------------------------------------
echo "> packing plugin"

mkdir -p                                                "${OflpInstallDir}"

cp  "./plugin/${TargetId}/linux/OpenFilesListPlus.so"   "${OflpInstallDir}"

rm -f  "./plugin/${TargetId}/linux/OpenFilesListPlus.so"
rm -f "${OflpInstallDir}/OpenFilesListPlus.zip"
rm -f "${OflpInstallDir}/OpenFilesListPlus.cbplugin"

zip -j9 "${OflpInstallDir}/OpenFilesListPlus.zip"       src/manifest.xml src/oflp.xrc graphics/oflp-panel-header-button-bar-padding.png graphics/oflp-panel-header-button-bar-blue.png graphics/oflp-panel-header-button-bar-green.png graphics/oflp-panel-header-button-bar-orange.png graphics/oflp-panel-header-button-bar-red.png  graphics/oflp-panel-header-button-bar-up.png graphics/oflp-panel-header-button-bar-down.png
zip -j9 "${OflpInstallDir}/OpenFilesListPlus.cbplugin"  "${OflpInstallDir}/OpenFilesListPlus.so" "${OflpInstallDir}/OpenFilesListPlus.zip"

#   overwrite "standalone" for debugging
echo "> updating plugin from [${OflpInstallDir}] to [${OflpSystemDir}]"
#cp -f "${OflpInstallDir}/OpenFilesListPlus.zip" "${OflpSystemDir}/codeblocks"
cp -f "${OflpInstallDir}/OpenFilesListPlus.so"  "${OflpSystemDir}/lib/codeblocks/plugins"

echo "> updating plugin from [${OflpInstallDir}] to [/home/gwr/.local/share/codeblocks/plugins/]"
cp -f "${OflpInstallDir}/OpenFilesListPlus.so"  "/home/gwr/.local/share/codeblocks/plugins/OpenFilesListPlus.so"
