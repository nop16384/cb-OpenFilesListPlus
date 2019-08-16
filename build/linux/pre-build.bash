#!/bin/bash

####################################################################################################
#
#   src/common/pre-build.bash
#
#   $1  CB PROJECT_DIR
#
####################################################################################################
ProjectDir="$1"

SrcDir="${ProjectDir}/src"
CciDir="${ProjectDir}/src/generated"

CciStrN=""
CciStrO=""

CciFilePath=""
####################################################################################################
function    f_create_new_cci_str
{
    local   d f
    local   a s
    #   ............................................................................................
    d="$1"
    f="$2"
    s=""
    a="LOG TKI TKW TKE INF WNG ERR"

    #echo "file[${f}]"

    s="${s}#ifdef   ${d}\n"
    for i in ${a} ; do
    s="${s}    #define ERG_${i}(FORMAT, ...)    ERGCB_${i}(FORMAT, __VA_ARGS__);\n"
    done

    s="${s}#else\n"
    for i in ${a} ; do
    s="${s}    #define ERG_${i}(FORMAT, ...)\n"
    done

    s="${s}#endif\n"

    CciStrN="${s}"
    return 0
}
####################################################################################################
echo "Pre-build.bash...[${TEST}]"

F1="${ProjectDir}/build/log-defines.txt"

while read -d $'\n' dn fn
do

    if [[ -z "${dn}" ]] ; then continue ; fi
    if [[ -z "${fn}" ]] ; then continue ; fi

    #echo "def =>[${dn}]"
    #echo "file=>[${fn}]"

    CciFilePath="${CciDir}/${fn}--log-defines.cci"

    f_create_new_cci_str    "${dn}"  "${CciDir}/${fn}--log-defines.cci"

    if [[ -f "${CciFilePath}" ]] ; then

        CciStrO=$( cat  "${CciFilePath}" )                                                          # chr(10) are transformed into spaces by bash ...

        sn=$(echo -e "${CciStrN}" )                                                                 # so do the same her, chr(10) -> space ...
        so="${CciStrO}"

        if [[ "${so}" == "${sn}" ]] ; then                                                          # so we can compare

            echo "  unchanged file [${CciFilePath}]..."

        else

            echo "${CciStrN}" > "${CciFilePath}"
            echo "  updated   file [${CciFilePath}]..."

        fi

    else

            echo "${CciStrN}" > "${CciFilePath}"
            echo "  created   file [${CciFilePath}]..."

    fi

done < "${F1}"



