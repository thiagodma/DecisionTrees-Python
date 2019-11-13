cp OnlineTranscoders/TTrEngine_BasketballDrive.cpp TTrEngine.cpp
cp TTrEngine.cpp HM13_rc1_Transcoder_Thiago_v2/source/Lib/TLibTranscoder
cd HM13_rc1_Transcoder_Thiago_v2/build/linux
make
cd ~
cp HM13_rc1_Transcoder_Thiago_v2/bin/TAppEncoderStatic ~/TAppEncoderStatic_v3
cp TAppEncoderStatic_v3 HEVC_CTC/TAppEncoderStatic
cd HEVC_CTC
./copy_new_enc_classb.sh
./run_basketballdrive.sh
cd ~

cp OnlineTranscoders/TTrEngine_BQTerrace.cpp TTrEngine.cpp
cp TTrEngine.cpp HM13_rc1_Transcoder_Thiago_v2/source/Lib/TLibTranscoder
cd HM13_rc1_Transcoder_Thiago_v2/build/linux
make
cd ~
cp HM13_rc1_Transcoder_Thiago_v2/bin/TAppEncoderStatic ~/TAppEncoderStatic_v3
cp TAppEncoderStatic_v3 HEVC_CTC/TAppEncoderStatic
cd HEVC_CTC
./copy_new_enc_classb.sh
./run_bqterrace.sh
cd ~

cp OnlineTranscoders/TTrEngine_Cactus.cpp TTrEngine.cpp
cp TTrEngine.cpp HM13_rc1_Transcoder_Thiago_v2/source/Lib/TLibTranscoder
cd HM13_rc1_Transcoder_Thiago_v2/build/linux
make
cd ~
cp HM13_rc1_Transcoder_Thiago_v2/bin/TAppEncoderStatic ~/TAppEncoderStatic_v3
cp TAppEncoderStatic_v3 HEVC_CTC/TAppEncoderStatic
cd HEVC_CTC
./copy_new_enc_classb.sh
./run_cactus.sh
cd ~

cp OnlineTranscoders/TTrEngine_Kimono.cpp TTrEngine.cpp
cp TTrEngine.cpp HM13_rc1_Transcoder_Thiago_v2/source/Lib/TLibTranscoder
cd HM13_rc1_Transcoder_Thiago_v2/build/linux
make
cd ~
cp HM13_rc1_Transcoder_Thiago_v2/bin/TAppEncoderStatic ~/TAppEncoderStatic_v3
cp TAppEncoderStatic_v3 HEVC_CTC/TAppEncoderStatic
cd HEVC_CTC
./copy_new_enc_classb.sh
./run_kimono.sh
cd ~

cp OnlineTranscoders/TTrEngine_ParkScene.cpp TTrEngine.cpp
cp TTrEngine.cpp HM13_rc1_Transcoder_Thiago_v2/source/Lib/TLibTranscoder
cd HM13_rc1_Transcoder_Thiago_v2/build/linux
make
cd ~
cp HM13_rc1_Transcoder_Thiago_v2/bin/TAppEncoderStatic ~/TAppEncoderStatic_v3
cp TAppEncoderStatic_v3 HEVC_CTC/TAppEncoderStatic
cd HEVC_CTC
./copy_new_enc_classb.sh
./run_parkscene.sh
cd ~

cd HEVC_CTC/Matlab_Transcoder/matlab_src
octave realPSNR_transcoder_all.m
