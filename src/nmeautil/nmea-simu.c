#include <stdint.h>

static uint8_t nmea_simu[] = "\
$GPRMC,081101.664,V,3854.931,N,07702.498,W,37.0,3.54,291217,,E*40\r\n\
$GPGGA,081102.664,3854.931,N,07702.498,W,0,00,,,M,,M,,*59\r\n\
$GPGLL,3854.931,N,07702.498,W,081103.664,V*38\r\n\
$GPVTG,3.54,T,,M,37.0,N,68.5,K*5D\r\n\
$GPRMC,081105.664,V,3854.932,N,07702.500,W,59.8,3.45,291217,,E*47\r\n\
$GPGGA,081106.664,3854.932,N,07702.500,W,0,00,,,M,,M,,*5E\r\n\
$GPGLL,3854.932,N,07702.500,W,081107.664,V*3F\r\n\
$GPVTG,3.45,T,,M,59.8,N,110.7,K*61\r\n\
$GPRMC,081109.664,V,3854.934,N,07702.501,W,73.4,3.55,291217,,E*49\r\n\
$GPGGA,081110.664,3854.934,N,07702.501,W,0,00,,,M,,M,,*5E\r\n\
$GPGLL,3854.934,N,07702.501,W,081111.664,V*3F\r\n\
$GPVTG,3.55,T,,M,73.4,N,135.9,K*6D\r\n\
$GPGSA,A,2,15,13,05,01,16,03,,,,,,,0.9,0.9,0.8*3D\r\n\
$GPGSV,2,1,06,15,36,353,60,13,35,306,15,05,13,257,24,01,54,240,67*7D\r\n\
$GPGSV,2,2,06,16,17,007,75,03,77,322,46*79\r\n\
$GPRMC,081116.664,V,3854.935,N,07702.502,W,0.4,3.61,291217,,E*76\r\n\
$GPGGA,081117.664,3854.935,N,07702.502,W,0,04,0.9,,M,,M,,*78\r\n\
$GPGLL,3854.935,N,07702.502,W,081118.664,V*34\r\n\
$GPVTG,3.61,T,,M,0.4,N,0.7,K*57\r\n\
$GPRMC,081120.664,V,3854.937,N,07702.502,W,3.0,4.10,291217,,E*77\r\n\
$GPGGA,081121.664,3854.937,N,07702.502,W,0,04,0.9,,M,,M,,*7F\r\n\
$GPGLL,3854.937,N,07702.502,W,081122.664,V*3F\r\n\
$GPVTG,4.10,T,,M,3.0,N,5.6,K*55\r\n\
$GPRMC,081124.664,V,3854.939,N,07702.502,W,56.8,3.99,291217,,E*43\r\n\
$GPGGA,081125.664,3854.939,N,07702.502,W,0,04,0.9,,M,,M,,*75\r\n\
$GPGLL,3854.939,N,07702.502,W,081126.664,V*35\r\n\
$GPVTG,3.99,T,,M,56.8,N,105.1,K*6D\r\n\
$GPRMC,081128.664,V,3854.940,N,07702.502,W,36.2,4.05,291217,,E*4F\r\n\
$GPGGA,081129.664,3854.940,N,07702.502,W,0,04,0.9,,M,,M,,*77\r\n\
$GPGLL,3854.940,N,07702.502,W,081130.664,V*3C\r\n\
$GPVTG,4.05,T,,M,36.2,N,67.0,K*57\r\n\
$GPRMC,081132.664,V,3854.942,N,07702.501,W,5.2,4.08,291217,,E*78\r\n\
$GPGGA,081133.664,3854.942,N,07702.501,W,0,04,0.9,,M,,M,,*7D\r\n\
$GPGLL,3854.942,N,07702.501,W,081134.664,V*39\r\n\
$GPVTG,4.08,T,,M,5.2,N,9.6,K*54\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.4,0.5,0.7*32\r\n\
$GPGSV,2,1,06,15,57,199,40,13,84,290,54,05,44,173,56,01,27,301,24*78\r\n\
$GPGSV,2,2,06,16,59,177,75,03,22,021,72*72\r\n\
$GPRMC,081139.664,V,3854.944,N,07702.501,W,99.9,4.12,291217,,E*40\r\n\
$GPGGA,081140.664,3854.944,N,07702.501,W,0,04,0.5,,M,,M,,*73\r\n\
$GPGLL,3854.944,N,07702.501,W,081141.664,V*3D\r\n\
$GPVTG,4.12,T,,M,99.9,N,185.1,K*63\r\n\
$GPRMC,081143.664,V,3854.946,N,07702.501,W,83.3,4.04,291217,,E*49\r\n\
$GPGGA,081144.664,3854.946,N,07702.501,W,0,04,0.5,,M,,M,,*75\r\n\
$GPGLL,3854.946,N,07702.501,W,081145.664,V*3B\r\n\
$GPVTG,4.04,T,,M,83.3,N,154.3,K*6B\r\n\
$GPRMC,081147.664,V,3854.948,N,07702.501,W,99.7,4.12,291217,,E*4B\r\n\
$GPGGA,081148.664,3854.948,N,07702.501,W,0,04,0.5,,M,,M,,*77\r\n\
$GPGLL,3854.948,N,07702.501,W,081149.664,V*39\r\n\
$GPVTG,4.12,T,,M,99.7,N,184.6,K*6B\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.9,0.9,1.0*35\r\n\
$GPGSV,2,1,06,15,38,316,55,13,55,318,48,05,56,108,57,01,53,014,28*76\r\n\
$GPGSV,2,2,06,16,76,259,61,03,57,060,21*74\r\n\
$GPRMC,081154.664,V,3854.949,N,07702.500,W,24.9,4.35,291217,,E*44\r\n\
$GPGGA,081155.664,3854.949,N,07702.500,W,0,04,0.9,,M,,M,,*77\r\n\
$GPGLL,3854.949,N,07702.500,W,081156.664,V*37\r\n\
$GPVTG,4.35,T,,M,24.9,N,46.1,K*5E\r\n\
$GPRMC,081158.664,V,3854.951,N,07702.499,W,7.6,4.38,291217,,E*73\r\n\
$GPGGA,081159.664,3854.951,N,07702.499,W,0,04,0.9,,M,,M,,*73\r\n\
$GPGLL,3854.951,N,07702.499,W,081200.664,V*3F\r\n\
$GPVTG,4.38,T,,M,7.6,N,14.0,K*6B\r\n\
$GPGSA,A,2,15,13,05,01,16,03,,,,,,,0.2,0.2,0.1*34\r\n\
$GPGSV,2,1,06,15,72,135,15,13,38,238,16,05,38,081,89,01,69,309,33*7B\r\n\
$GPGSV,2,2,06,16,06,264,32,03,12,222,27*78\r\n\
$GPRMC,081205.664,V,3854.952,N,07702.497,W,69.5,4.46,291217,,E*47\r\n\
$GPGGA,081206.664,3854.952,N,07702.497,W,0,04,0.2,,M,,M,,*7C\r\n\
$GPGLL,3854.952,N,07702.497,W,081207.664,V*35\r\n\
$GPVTG,4.46,T,,M,69.5,N,128.6,K*61\r\n\
$GPRMC,081209.664,V,3854.953,N,07702.496,W,57.2,4.52,291217,,E*44\r\n\
$GPGGA,081210.664,3854.953,N,07702.496,W,0,04,0.2,,M,,M,,*7B\r\n\
$GPGLL,3854.953,N,07702.496,W,081211.664,V*32\r\n\
$GPVTG,4.52,T,,M,57.2,N,105.9,K*6E\r\n\
$GPGSA,A,2,15,13,05,01,16,03,,,,,,,0.0,0.7,0.4*36\r\n\
$GPGSV,2,1,06,15,88,026,46,13,08,169,78,05,38,284,88,01,47,023,28*7C\r\n\
$GPGSV,2,2,06,16,75,040,88,03,52,315,65*7E\r\n\
$GPRMC,081216.664,V,3854.954,N,07702.495,W,82.4,4.69,291217,,E*48\r\n\
$GPGGA,081217.664,3854.954,N,07702.495,W,0,04,0.7,,M,,M,,*7D\r\n\
$GPGLL,3854.954,N,07702.495,W,081218.664,V*3F\r\n\
$GPVTG,4.69,T,,M,82.4,N,152.6,K*65\r\n\
$GPRMC,081220.664,V,3854.955,N,07702.493,W,77.7,4.79,291217,,E*42\r\n\
$GPGGA,081221.664,3854.955,N,07702.493,W,0,04,0.7,,M,,M,,*7F\r\n\
$GPGLL,3854.955,N,07702.493,W,081222.664,V*31\r\n\
$GPVTG,4.79,T,,M,77.7,N,143.8,K*63\r\n\
$GPRMC,081224.664,V,3854.956,N,07702.492,W,43.7,4.47,291217,,E*4E\r\n\
$GPGGA,081225.664,3854.956,N,07702.492,W,0,04,0.7,,M,,M,,*79\r\n\
$GPGLL,3854.956,N,07702.492,W,081226.664,V*37\r\n\
$GPVTG,4.47,T,,M,43.7,N,81.0,K*5E\r\n\
$GPRMC,081228.664,V,3854.957,N,07702.490,W,44.6,4.55,291217,,E*44\r\n\
$GPGGA,081229.664,3854.957,N,07702.490,W,0,04,0.7,,M,,M,,*76\r\n\
$GPGLL,3854.957,N,07702.490,W,081230.664,V*33\r\n\
$GPVTG,4.55,T,,M,44.6,N,82.6,K*5E\r\n\
$GPRMC,081232.664,V,3854.958,N,07702.489,W,85.8,4.57,291217,,E*49\r\n\
$GPGGA,081233.664,3854.958,N,07702.489,W,0,04,0.7,,M,,M,,*7A\r\n\
$GPGLL,3854.958,N,07702.489,W,081234.664,V*30\r\n\
$GPVTG,4.57,T,,M,85.8,N,158.9,K*66\r\n\
$GPRMC,081236.664,V,3854.959,N,07702.487,W,95.8,4.68,291217,,E*4F\r\n\
$GPGGA,081237.664,3854.959,N,07702.487,W,0,04,0.7,,M,,M,,*71\r\n\
$GPGLL,3854.959,N,07702.487,W,081238.664,V*33\r\n\
$GPVTG,4.68,T,,M,95.8,N,177.4,K*6B\r\n\
$GPRMC,081240.664,V,3854.960,N,07702.486,W,35.8,4.67,291217,,E*40\r\n\
$GPGGA,081241.664,3854.960,N,07702.486,W,0,04,0.7,,M,,M,,*7B\r\n\
$GPGLL,3854.960,N,07702.486,W,081242.664,V*35\r\n\
$GPVTG,4.67,T,,M,35.8,N,66.3,K*58\r\n\
$GPRMC,081244.664,V,3854.961,N,07702.484,W,23.3,4.76,291217,,E*4B\r\n\
$GPGGA,081245.664,3854.961,N,07702.484,W,0,04,0.7,,M,,M,,*7C\r\n\
$GPGLL,3854.961,N,07702.484,W,081246.664,V*32\r\n\
$GPVTG,4.76,T,,M,23.3,N,43.2,K*52\r\n\
$GPRMC,081248.664,V,3854.962,N,07702.483,W,21.6,4.53,291217,,E*43\r\n\
$GPGGA,081249.664,3854.962,N,07702.483,W,0,04,0.7,,M,,M,,*74\r\n\
$GPGLL,3854.962,N,07702.483,W,081250.664,V*31\r\n\
$GPVTG,4.53,T,,M,21.6,N,40.0,K*53\r\n\
$GPRMC,081252.664,V,3854.962,N,07702.481,W,17.7,4.79,291217,,E*46\r\n\
$GPGGA,081253.664,3854.962,N,07702.481,W,0,04,0.7,,M,,M,,*7D\r\n\
$GPGLL,3854.962,N,07702.481,W,081254.664,V*37\r\n\
$GPVTG,4.79,T,,M,17.7,N,32.7,K*5D\r\n\
$GPRMC,081256.664,V,3854.963,N,07702.479,W,72.6,4.71,291217,,E*4E\r\n\
$GPGGA,081257.664,3854.963,N,07702.479,W,0,04,0.7,,M,,M,,*7F\r\n\
$GPGLL,3854.963,N,07702.479,W,081258.664,V*3D\r\n\
$GPVTG,4.71,T,,M,72.6,N,134.5,K*62\r\n\
$GPRMC,081300.664,V,3854.964,N,07702.478,W,82.7,4.70,291217,,E*45\r\n\
$GPGGA,081301.664,3854.964,N,07702.478,W,0,04,0.7,,M,,M,,*7B\r\n\
$GPGLL,3854.964,N,07702.478,W,081302.664,V*35\r\n\
$GPVTG,4.70,T,,M,82.7,N,153.2,K*6B\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.2,0.2,0.1*35\r\n\
$GPGSV,2,1,06,15,43,115,59,13,25,102,65,05,87,037,59,01,52,238,85*73\r\n\
$GPGSV,2,2,06,16,12,189,45,03,34,331,33*7F\r\n\
$GPRMC,081307.664,V,3854.965,N,07702.476,W,89.6,4.72,291217,,E*45\r\n\
$GPGGA,081308.664,3854.965,N,07702.476,W,0,04,0.2,,M,,M,,*78\r\n\
$GPGLL,3854.965,N,07702.476,W,081309.664,V*31\r\n\
$GPVTG,4.72,T,,M,89.6,N,166.0,K*67\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.4,0.2,0.0*32\r\n\
$GPGSV,2,1,06,15,80,166,81,13,75,270,27,05,16,118,70,01,64,213,51*72\r\n\
$GPGSV,2,2,06,16,23,121,19,03,20,198,48*7E\r\n\
$GPRMC,081314.664,V,3854.965,N,07702.475,W,9.9,4.77,291217,,E*76\r\n\
$GPGGA,081315.664,3854.965,N,07702.475,W,0,04,0.2,,M,,M,,*77\r\n\
$GPGLL,3854.965,N,07702.475,W,081316.664,V*3C\r\n\
$GPVTG,4.77,T,,M,9.9,N,18.3,K*6E\r\n\
$GPRMC,081318.664,V,3854.966,N,07702.473,W,84.8,4.67,291217,,E*4A\r\n\
$GPGGA,081319.664,3854.966,N,07702.473,W,0,04,0.2,,M,,M,,*7E\r\n\
$GPGLL,3854.966,N,07702.473,W,081320.664,V*3C\r\n\
$GPVTG,4.67,T,,M,84.8,N,157.1,K*63\r\n\
$GPRMC,081322.664,V,3854.967,N,07702.471,W,89.4,4.72,291217,,E*45\r\n\
$GPGGA,081323.664,3854.967,N,07702.471,W,0,04,0.2,,M,,M,,*74\r\n\
$GPGLL,3854.967,N,07702.471,W,081324.664,V*3B\r\n\
$GPVTG,4.72,T,,M,89.4,N,165.5,K*63\r\n\
$GPRMC,081326.664,V,3854.968,N,07702.470,W,72.4,4.61,291217,,E*49\r\n\
$GPGGA,081327.664,3854.968,N,07702.470,W,0,04,0.2,,M,,M,,*7E\r\n\
$GPGLL,3854.968,N,07702.470,W,081328.664,V*39\r\n\
$GPVTG,4.61,T,,M,72.4,N,134.2,K*66\r\n\
$GPRMC,081330.664,V,3854.969,N,07702.468,W,36.3,4.59,291217,,E*4A\r\n\
$GPGGA,081331.664,3854.969,N,07702.468,W,0,04,0.2,,M,,M,,*71\r\n\
$GPGLL,3854.969,N,07702.468,W,081332.664,V*3A\r\n\
$GPVTG,4.59,T,,M,36.3,N,67.2,K*5D\r\n\
$GPRMC,081334.664,V,3854.970,N,07702.467,W,12.0,4.57,291217,,E*42\r\n\
$GPGGA,081335.664,3854.970,N,07702.467,W,0,04,0.2,,M,,M,,*72\r\n\
$GPGLL,3854.970,N,07702.467,W,081336.664,V*39\r\n\
$GPVTG,4.57,T,,M,12.0,N,22.1,K*54\r\n\
$GPGSA,A,2,15,13,05,01,16,03,,,,,,,0.6,0.7,0.2*36\r\n\
$GPGSV,2,1,06,15,68,185,19,13,58,263,83,05,56,243,33,01,49,076,88*7F\r\n\
$GPGSV,2,2,06,16,81,196,37,03,49,107,49*7E\r\n\
$GPRMC,081341.664,V,3854.972,N,07702.466,W,89.1,4.51,291217,,E*46\r\n\
$GPGGA,081342.664,3854.972,N,07702.466,W,0,04,0.7,,M,,M,,*74\r\n\
$GPGLL,3854.972,N,07702.466,W,081343.664,V*38\r\n\
$GPVTG,4.51,T,,M,89.1,N,165.1,K*63\r\n\
$GPRMC,081345.664,V,3854.973,N,07702.464,W,62.8,4.49,291217,,E*44\r\n\
$GPGGA,081346.664,3854.973,N,07702.464,W,0,04,0.7,,M,,M,,*73\r\n\
$GPGLL,3854.973,N,07702.464,W,081347.664,V*3F\r\n\
$GPVTG,4.49,T,,M,62.8,N,116.2,K*61\r\n\
$GPRMC,081349.664,V,3854.974,N,07702.463,W,43.1,4.60,291217,,E*49\r\n\
$GPGGA,081350.664,3854.974,N,07702.463,W,0,04,0.7,,M,,M,,*74\r\n\
$GPGLL,3854.974,N,07702.463,W,081351.664,V*38\r\n\
$GPVTG,4.60,T,,M,43.1,N,79.8,K*52\r\n\
$GPRMC,081353.664,V,3854.975,N,07702.462,W,53.2,4.50,291217,,E*43\r\n\
$GPGGA,081354.664,3854.975,N,07702.462,W,0,04,0.7,,M,,M,,*70\r\n\
$GPGLL,3854.975,N,07702.462,W,081355.664,V*3C\r\n\
$GPVTG,4.50,T,,M,53.2,N,98.6,K*52\r\n\
$GPRMC,081357.664,V,3854.977,N,07702.461,W,14.1,4.48,291217,,E*4F\r\n\
$GPGGA,081358.664,3854.977,N,07702.461,W,0,04,0.7,,M,,M,,*7D\r\n\
$GPGLL,3854.977,N,07702.461,W,081359.664,V*31\r\n\
$GPVTG,4.48,T,,M,14.1,N,26.1,K*59\r\n\
$GPRMC,081401.664,V,3854.978,N,07702.459,W,67.7,4.43,291217,,E*46\r\n\
$GPGGA,081402.664,3854.978,N,07702.459,W,0,04,0.7,,M,,M,,*71\r\n\
$GPGLL,3854.978,N,07702.459,W,081403.664,V*3D\r\n\
$GPVTG,4.43,T,,M,67.7,N,125.4,K*67\r\n\
$GPRMC,081405.664,V,3854.979,N,07702.458,W,68.3,4.44,291217,,E*4E\r\n\
$GPGGA,081406.664,3854.979,N,07702.458,W,0,04,0.7,,M,,M,,*75\r\n\
$GPGLL,3854.979,N,07702.458,W,081407.664,V*39\r\n\
$GPVTG,4.44,T,,M,68.3,N,126.5,K*69\r\n\
$GPRMC,081409.664,V,3854.981,N,07702.457,W,47.8,4.41,291217,,E*49\r\n\
$GPGGA,081410.664,3854.981,N,07702.457,W,0,04,0.7,,M,,M,,*7A\r\n\
$GPGLL,3854.981,N,07702.457,W,081411.664,V*36\r\n\
$GPVTG,4.41,T,,M,47.8,N,88.5,K*5F\r\n\
$GPRMC,081413.664,V,3854.982,N,07702.456,W,75.3,4.48,291217,,E*43\r\n\
$GPGGA,081414.664,3854.982,N,07702.456,W,0,04,0.7,,M,,M,,*7C\r\n\
$GPGLL,3854.982,N,07702.456,W,081415.664,V*30\r\n\
$GPVTG,4.48,T,,M,75.3,N,139.5,K*67\r\n\
$GPRMC,081417.664,V,3854.984,N,07702.455,W,12.0,4.37,291217,,E*48\r\n\
$GPGGA,081418.664,3854.984,N,07702.455,W,0,04,0.7,,M,,M,,*75\r\n\
$GPGLL,3854.984,N,07702.455,W,081419.664,V*39\r\n\
$GPVTG,4.37,T,,M,12.0,N,22.3,K*50\r\n\
$GPRMC,081421.664,V,3854.985,N,07702.454,W,89.0,4.28,291217,,E*41\r\n\
$GPGGA,081422.664,3854.985,N,07702.454,W,0,04,0.7,,M,,M,,*7C\r\n\
$GPGLL,3854.985,N,07702.454,W,081423.664,V*30\r\n\
$GPVTG,4.28,T,,M,89.0,N,164.9,K*65\r\n\
$GPRMC,081425.664,V,3854.987,N,07702.455,W,91.6,3.82,291217,,E*4E\r\n\
$GPGGA,081426.664,3854.987,N,07702.455,W,0,04,0.7,,M,,M,,*7B\r\n\
$GPGLL,3854.987,N,07702.455,W,081427.664,V*37\r\n\
$GPVTG,3.82,T,,M,91.6,N,169.7,K*6E\r\n\
$GPRMC,081429.664,V,3854.989,N,07702.455,W,43.8,3.83,291217,,E*4C\r\n\
$GPGGA,081430.664,3854.989,N,07702.455,W,0,04,0.7,,M,,M,,*72\r\n\
$GPGLL,3854.989,N,07702.455,W,081431.664,V*3E\r\n\
$GPVTG,3.83,T,,M,43.8,N,81.2,K*5C\r\n\
$GPRMC,081433.664,V,3854.990,N,07702.456,W,58.2,3.80,291217,,E*4F\r\n\
$GPGGA,081434.664,3854.990,N,07702.456,W,0,04,0.7,,M,,M,,*7D\r\n\
$GPGLL,3854.990,N,07702.456,W,081435.664,V*31\r\n\
$GPVTG,3.80,T,,M,58.2,N,107.7,K*65\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.5,0.9,0.5*3D\r\n\
$GPGSV,2,1,06,15,36,182,59,13,19,273,89,05,05,108,64,01,69,192,45*79\r\n\
$GPGSV,2,2,06,16,78,056,58,03,80,008,84*76\r\n\
$GPRMC,081440.664,V,3854.992,N,07702.456,W,55.0,3.82,291217,,E*44\r\n\
$GPGGA,081441.664,3854.992,N,07702.456,W,0,04,0.9,,M,,M,,*73\r\n\
$GPGLL,3854.992,N,07702.456,W,081442.664,V*33\r\n\
$GPVTG,3.82,T,,M,55.0,N,101.9,K*60\r\n\
$GPRMC,081444.664,V,3854.994,N,07702.457,W,53.7,3.77,291217,,E*4C\r\n\
$GPGGA,081445.664,3854.994,N,07702.457,W,0,04,0.9,,M,,M,,*70\r\n\
$GPGLL,3854.994,N,07702.457,W,081446.664,V*30\r\n\
$GPVTG,3.77,T,,M,53.7,N,99.4,K*56\r\n\
$GPGSA,A,3,15,13,05,01,16,03,,,,,,,0.9,0.3,0.1*3F\r\n\
$GPGSV,2,1,06,15,28,015,46,13,58,250,18,05,29,120,70,01,48,177,49*7E\r\n\
$GPGSV,2,2,06,16,67,258,70,03,14,057,66*75\r\n\
";