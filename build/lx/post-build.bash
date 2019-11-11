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
#   ------------------------------------------------------------------------------------------------
if [[ "${OflpWxVersion}" == "3.0" ]] ; then

    OflpWxVersion="3.0.4"

fi

if [[ "${OflpWxVersion}" == "2.8" ]] ; then

    OflpWxVersion="2.8.12"

fi

echo "> Options"
echo "  target id  [${TargetId}]"
echo "  wx version [${OflpWxVersion}]"
echo "  wx toolkit [${OflpWxToolkit}]"
#   ------------------------------------------------------------------------------------------------
OflpOutputDir="./plugin/lx/${TargetId}/wx-${OflpWxVersion}/${OflpWxToolkit}"
echo "  plugin output directory [${OflpOutputDir}]"

OflpSystemDir="/usr/local/gwr/codeblocks/standalone/svn11267/wx-${OflpWxVersion}"
echo "  installation  directory [${OflpSystemDir}]"
#   ------------------------------------------------------------------------------------------------
echo "> packing plugin"

mkdir -p                                                "${OflpOutputDir}"

cp  "./plugin/lx/${TargetId}/OpenFilesListPlus.so"      "${OflpOutputDir}"

rm -f  "./plugin/lx/${TargetId}/OpenFilesListPlus.so"
rm -f "${OflpOutputDir}/OpenFilesListPlus.zip"
rm -f "${OflpOutputDir}/OpenFilesListPlus.cbplugin"

zip -j9 "${OflpOutputDir}/OpenFilesListPlus.zip"       src/manifest.xml src/oflp.xrc graphics/oflp-panel-header-button-bar-padding.png graphics/oflp-panel-header-button-bar-blue.png graphics/oflp-panel-header-button-bar-green.png graphics/oflp-panel-header-button-bar-orange.png graphics/oflp-panel-header-button-bar-red.png  graphics/oflp-panel-header-button-bar-up.png graphics/oflp-panel-header-button-bar-down.png
zip -j9 "${OflpOutputDir}/OpenFilesListPlus.cbplugin"  "${OflpOutputDir}/OpenFilesListPlus.so" "${OflpOutputDir}/OpenFilesListPlus.zip"

#   overwrite "standalone" for debugging
echo "> updating plugin from [${OflpOutputDir}] to [${OflpSystemDir}]"
#cp -f "${OflpOutputDir}/OpenFilesListPlus.zip" "${OflpSystemDir}/codeblocks"
cp -f "${OflpOutputDir}/OpenFilesListPlus.so"  "${OflpSystemDir}/lib/codeblocks/plugins"

echo "> updating plugin from [${OflpOutputDir}] to [/home/gwr/.local/share/codeblocks/plugins/]"
cp -f "${OflpOutputDir}/OpenFilesListPlus.so"  "/home/gwr/.local/share/codeblocks/plugins/OpenFilesListPlus.so"

