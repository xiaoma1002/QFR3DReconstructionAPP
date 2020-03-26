#-------------------------------------------------
#
# Project created by QtCreator 2019-07-09T12:01:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFR3DReconstructionApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    CameraPinHole.cpp \
    IntrinsicParameter.cpp \
    ExtrinsicParameter.cpp \
    ThinImage.cpp \
    ContourDetector.cpp \
    DualCameraSystem.cpp \
    Pixel.cpp \
    ExtractSingleCurveFromImage.cpp \
    ValidPointDetector.cpp \
    ValidPointGroup.cpp \
    Triangulation.cpp \
    ExtractTwoEdgesFromContour.cpp \
    SurfaceReconstruction.cpp \
    QFRDebugger.cpp \
    FindEpiloarIntersectionForPair.cpp \
    FindUniqueIntersectionLineAndCurve.cpp \
    PixelUVImageXYTransformer.cpp \
    PixelCurve.cpp \
    PixelToLineDistanceMeasurer.cpp \
    EstimateNormalDirectionPixelCurve.cpp \
    DualCameraSystem2.cpp \
    ValidPointDetector2.cpp \
    SurfaceReconstruction2.cpp \
    ValidPointGroup2.cpp \
    FindEdgePointForCamera.cpp \
    CrossSectionProfile.cpp

HEADERS += \
        mainwindow.h \
    CameraPinHole.h \
    IntrinsicParameter.h \
    ExtrinsicParameter.h \
    ThinImage.h \
    ContourDetector.h \
    DualCameraSystem.h \
    Pixel.h \
    ExtractSingleCurveFromImage.h \
    ValidPointDetector.h \
    ValidPointGroup.h \
    Triangulation.h \
    ExtractTwoEdgesFromContour.h \
    SurfaceReconstruction.h \
    QFRDebugger.h \
    SortMatrix.h \
    FindEpiloarIntersectionForPair.h \
    FindUniqueIntersectionLineAndCurve.h \
    PixelUVImageXYTransformer.h \
    stl_reader.h \
    PixelCurve.h \
    PixelToLineDistanceMeasurer.h \
    EstimateNormalDirectionPixelCurve.h \
    DualCameraSystem2.h \
    ValidPointDetector2.h \
    SurfaceReconstruction2.h \
    ValidPointGroup2.h \
    FindEdgePointForCamera.h \
    CrossSectionProfile.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:/OpenCV/OpenCV-4.1.1/build/install/include
INCLUDEPATH += D:/3DReconstruction/QFR3DReconstructionApp/Eigen

#LIBS += -LD:/OpenCV/OpenCV-4.1.1/build/install/x64/vc15/lib/


INCLUDEPATH += D:/OpenCV/OpenCV-4.1.1/build-debug/install/include
DEPENDPATH += D:/OpenCV/OpenCV-4.1.1/build-debug/install/include

win32:CONFIG(release, debug|release): {
LIBS += -LD:/OpenCV/OpenCV-4.1.1/build/install/x64/vc15/lib \
    -lopencv_aruco410 \
    -lopencv_bgsegm410 \
    -lopencv_bioinspired410 \
    -lopencv_calib3d410 \
    -lopencv_ccalib410 \
    -lopencv_core410 \
    -lopencv_datasets410 \
    -lopencv_dnn410 \
    -lopencv_dnn_objdetect410 \
    -lopencv_dpm410 \
    -lopencv_face410 \
    -lopencv_features2d410 \
    -lopencv_flann410 \
    -lopencv_fuzzy410 \
    -lopencv_gapi410 \
    -lopencv_hdf410 \
    -lopencv_hfs410 \
    -lopencv_highgui410 \
    -lopencv_imgcodecs410 \
    -lopencv_imgproc410 \
    -lopencv_img_hash410 \
    -lopencv_line_descriptor410 \
    -lopencv_ml410 \
    -lopencv_objdetect410 \
    -lopencv_optflow410 \
    -lopencv_phase_unwrapping410 \
    -lopencv_photo410 \
    -lopencv_plot410 \
    -lopencv_quality410 \
    -lopencv_reg410 \
    -lopencv_rgbd410 \
    -lopencv_saliency410 \
    -lopencv_shape410 \
    -lopencv_stereo410 \
    -lopencv_stitching410 \
    -lopencv_structured_light410 \
    -lopencv_superres410 \
    -lopencv_surface_matching410 \
    -lopencv_text410 \
    -lopencv_tracking410 \
    -lopencv_video410 \
    -lopencv_videoio410 \
    -lopencv_videostab410 \
    -lopencv_xfeatures2d410 \
    -lopencv_ximgproc410 \
    -lopencv_xobjdetect410 \
    -lopencv_xphoto410
}
else:win32:CONFIG(debug, debug|release):{
LIBS += -LD:/OpenCV/OpenCV-4.1.1/build-debug/install/x64/vc15/lib/ \
    -lopencv_aruco410d \
    -lopencv_bgsegm410d \
    -lopencv_bioinspired410d \
    -lopencv_calib3d410d \
    -lopencv_ccalib410d \
    -lopencv_core410d \
    -lopencv_datasets410d \
    -lopencv_dnn410d \
    -lopencv_dnn_objdetect410d \
    -lopencv_dpm410d \
    -lopencv_face410d \
    -lopencv_features2d410d \
    -lopencv_flann410d \
    -lopencv_fuzzy410d \
    -lopencv_gapi410d \
    -lopencv_hdf410d \
    -lopencv_hfs410d \
    -lopencv_highgui410d \
    -lopencv_imgcodecs410d \
    -lopencv_imgproc410d \
    -lopencv_img_hash410 \
    -lopencv_img_hash410d \
    -lopencv_line_descriptor410d \
    -lopencv_ml410d \
    -lopencv_objdetect410d \
    -lopencv_optflow410d \
    -lopencv_phase_unwrapping410d \
    -lopencv_photo410d \
    -lopencv_plot410d \
    -lopencv_quality410d \
    -lopencv_reg410d \
    -lopencv_rgbd410d \
    -lopencv_saliency410d \
    -lopencv_shape410d \
    -lopencv_stereo410d \
    -lopencv_stitching410d \
    -lopencv_structured_light410d \
    -lopencv_superres410d \
    -lopencv_surface_matching410d \
    -lopencv_text410d \
    -lopencv_tracking410d \
    -lopencv_video410d \
    -lopencv_videoio410d \
    -lopencv_videostab410d \
    -lopencv_world410 \
    -lopencv_world410d \
    -lopencv_xfeatures2d410d \
    -lopencv_ximgproc410d \
    -lopencv_xobjdetect410d \
    -lopencv_xphoto410d \
    -lopencv_world410d
}

INCLUDEPATH += D:/OpenCV/OpenCV-4.1.1/build-debug/install/x64/vc15
DEPENDPATH += D:/OpenCV/OpenCV-4.1.1/build-debug/install/x64/vc15
