cd('..')
startup;
cd('matlab_src')

%==========================================================================
%BasketballDrive

width = 1920;
height = 1080;

original_file = '../../../Sequences/HEVC_CTC/ClassB/BasketballDrive_1920x1080_50_8bit_420.yuv';
nframes = 500;

test_file = '../../ClassB/BasketballDrive/QP_22/HEVC_trivial/rec.yuv';
avgPSNR_BasketballDrive(1) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BasketballDrive/QP_27/HEVC_trivial/rec.yuv';
avgPSNR_BasketballDrive(2) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BasketballDrive/QP_32/HEVC_trivial/rec.yuv';
avgPSNR_BasketballDrive(3) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BasketballDrive/QP_37/HEVC_trivial/rec.yuv';
avgPSNR_BasketballDrive(4) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

%==========================================================================
%==========================================================================
%BQTerrace

width = 1920;
height = 1080;

original_file = '../../../Sequences/HEVC_CTC/ClassB/BQTerrace_1920x1080_60_8bit_420.yuv';
nframes = 600;

test_file = '../../ClassB/BQTerrace/QP_22/HEVC_trivial/rec.yuv';
avgPSNR_BQTerrace(1) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BQTerrace/QP_27/HEVC_trivial/rec.yuv';
avgPSNR_BQTerrace(2) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BQTerrace/QP_32/HEVC_trivial/rec.yuv';
avgPSNR_BQTerrace(3) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/BQTerrace/QP_37/HEVC_trivial/rec.yuv';
avgPSNR_BQTerrace(4) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

%==========================================================================
%==========================================================================
%Cactus

width = 1920;
height = 1080;

original_file = '../../../Sequences/HEVC_CTC/ClassB/Cactus_1920x1080_50_8bit_420.yuv';
nframes = 500;

test_file = '../../ClassB/Cactus/QP_22/HEVC_trivial/rec.yuv';
avgPSNR_Cactus(1) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Cactus/QP_27/HEVC_trivial/rec.yuv';
avgPSNR_Cactus(2) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Cactus/QP_32/HEVC_trivial/rec.yuv';
avgPSNR_Cactus(3) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Cactus/QP_37/HEVC_trivial/rec.yuv';
avgPSNR_Cactus(4) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

%==========================================================================
%==========================================================================
%Kimono

width = 1920;
height = 1080;

original_file = '../../../Sequences/HEVC_CTC/ClassB/Kimono1_1920x1080_24_8bit_420.yuv';
nframes = 240;

test_file = '../../ClassB/Kimono/QP_22/HEVC_trivial/rec.yuv';
avgPSNR_Kimono(1) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Kimono/QP_27/HEVC_trivial/rec.yuv';
avgPSNR_Kimono(2) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Kimono/QP_32/HEVC_trivial/rec.yuv';
avgPSNR_Kimono(3) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/Kimono/QP_37/HEVC_trivial/rec.yuv';
avgPSNR_Kimono(4) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

%==========================================================================

%==========================================================================
%ParkScene

width = 1920;
height = 1080;

original_file = '../../../Sequences/HEVC_CTC/ClassB/ParkScene_1920x1080_24_8bit_420.yuv';
nframes = 240;

test_file = '../../ClassB/ParkScene/QP_22/HEVC_trivial/rec.yuv';
avgPSNR_ParkScene(1) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/ParkScene/QP_27/HEVC_trivial/rec.yuv';
avgPSNR_ParkScene(2) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/ParkScene/QP_32/HEVC_trivial/rec.yuv';
avgPSNR_ParkScene(3) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);

test_file = '../../ClassB/ParkScene/QP_37/HEVC_trivial/rec.yuv';
avgPSNR_ParkScene(4) = thiagoComputeAveragePSNR(original_file, test_file, width, height,nframes);


clear width height original_file nframes test_file
save('psnr_per_sequence_classb_offline')
