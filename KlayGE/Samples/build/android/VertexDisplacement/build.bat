call %ANDROID_NDK%\ndk-build

call ant debug
adb install -r bin\KlayGE_VertexDisplacement-debug.apk

adb push ../../../../bin/android_armeabi/KlayGE.cfg /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/Fonts/gkai00mp.kfont /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/2D.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/Font.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/Font_Font2DVS_0.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/Font_Font3DVS_0.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/Font_FontPS_1.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/GammaCorrection.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/GammaCorrection_PostProcessVS_0.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/GammaCorrection_GammaCorrectionPS_1.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/Lighting.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/PostProcess.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/UI.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/UI_UIVS_0.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/UI_UIPS_1.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/RenderFX/UI_UINoTexPS_1.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/PostProcessors/GammaCorrection.ppml /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/Textures/2D/powered_by_klayge.dds /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/Textures/2D/ui.dds /data/data/klayge.sample.vertexdisplacement
adb push ../../../../media/Textures/2D/powered_by_klayge.dds /data/data/klayge.sample.vertexdisplacement
adb push ../../../media/VertexDisplacement/VertexDisplacement.fxml /data/data/klayge.sample.vertexdisplacement
adb push ../../../media/VertexDisplacement/VertexDisplacement_VertexDisplacementPS_1.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../media/VertexDisplacement/VertexDisplacement_VertexDisplacementVS_0.fxml_bin /data/data/klayge.sample.vertexdisplacement
adb push ../../../media/VertexDisplacement/VertexDisplacement.uiml /data/data/klayge.sample.vertexdisplacement
