#!/bin/bash

if [ "$1" == "" ]; then
  if [ "$2" == "32" ]; then
    # set environment variables used by OCCT
    export CSF_FPE=0

    export TCL_DIR=""
    export TK_DIR=""
    export FREETYPE_DIR=""
    export FREEIMAGE_DIR=""
    export TBB_DIR=""
    export VTK_DIR=""
    export FFMPEG_DIR=""

    if [ "x" != "x" ]; then
      export QTDIR=""
    fi

    export TCL_VERSION_WITH_DOT=""
    export TK_VERSION_WITH_DOT=""

    export CSF_OCCTBinPath="${CASROOT}/bin"
    export CSF_OCCTLibPath="${CASROOT}/lib"
    export CSF_OCCTIncludePath="${CASROOT}/inc"
    export CSF_OCCTResourcePath="${CASROOT}/src"
    export CSF_OCCTDataPath="${CASROOT}/share/opencascade/data"
    export CSF_OCCTSamplesPath="${CASROOT}/share/opencascade/samples"
    export CSF_OCCTTestsPath="${CASROOT}/share/opencascade/tests"
    export CSF_OCCTDocPath="${CASROOT}/share/doc/opencascade"
  fi
fi

