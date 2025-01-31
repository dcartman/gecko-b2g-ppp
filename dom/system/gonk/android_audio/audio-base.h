// This file is autogenerated by hidl-gen. Do not edit manually.
// Source: android.hardware.audio.common@2.0
// Root: android.hardware:hardware/interfaces

#ifndef HIDL_GENERATED_ANDROID_HARDWARE_AUDIO_COMMON_V2_0_EXPORTED_CONSTANTS_H_
#define HIDL_GENERATED_ANDROID_HARDWARE_AUDIO_COMMON_V2_0_EXPORTED_CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
  AUDIO_IO_HANDLE_NONE = 0,
  AUDIO_MODULE_HANDLE_NONE = 0,
  AUDIO_PORT_HANDLE_NONE = 0,
  AUDIO_PATCH_HANDLE_NONE = 0,
};

typedef enum {
  AUDIO_STREAM_DEFAULT = -1,  // (-1)
  AUDIO_STREAM_MIN = 0,
  AUDIO_STREAM_VOICE_CALL = 0,
  AUDIO_STREAM_SYSTEM = 1,
  AUDIO_STREAM_RING = 2,
  AUDIO_STREAM_MUSIC = 3,
  AUDIO_STREAM_ALARM = 4,
  AUDIO_STREAM_NOTIFICATION = 5,
  AUDIO_STREAM_BLUETOOTH_SCO = 6,
  AUDIO_STREAM_ENFORCED_AUDIBLE = 7,
  AUDIO_STREAM_DTMF = 8,
  AUDIO_STREAM_TTS = 9,
  AUDIO_STREAM_ACCESSIBILITY = 10,
  AUDIO_STREAM_REROUTING = 11,
  AUDIO_STREAM_PATCH = 12,
  AUDIO_STREAM_PUBLIC_CNT = 11,      // (ACCESSIBILITY + 1)
  AUDIO_STREAM_FOR_POLICY_CNT = 12,  // PATCH
  AUDIO_STREAM_CNT = 13,             // (PATCH + 1)
} audio_stream_type_t;

typedef enum {
  AUDIO_SOURCE_DEFAULT = 0,
  AUDIO_SOURCE_MIC = 1,
  AUDIO_SOURCE_VOICE_UPLINK = 2,
  AUDIO_SOURCE_VOICE_DOWNLINK = 3,
  AUDIO_SOURCE_VOICE_CALL = 4,
  AUDIO_SOURCE_CAMCORDER = 5,
  AUDIO_SOURCE_VOICE_RECOGNITION = 6,
  AUDIO_SOURCE_VOICE_COMMUNICATION = 7,
  AUDIO_SOURCE_REMOTE_SUBMIX = 8,
  AUDIO_SOURCE_UNPROCESSED = 9,
  AUDIO_SOURCE_CNT = 10,
  AUDIO_SOURCE_MAX = 9,  // (CNT - 1)
  AUDIO_SOURCE_FM_TUNER = 1998,
  AUDIO_SOURCE_HOTWORD = 1999,
} audio_source_t;

typedef enum {
  AUDIO_SESSION_OUTPUT_STAGE = -1,  // (-1)
  AUDIO_SESSION_OUTPUT_MIX = 0,
  AUDIO_SESSION_ALLOCATE = 0,
  AUDIO_SESSION_NONE = 0,
} audio_session_t;

typedef enum {
  AUDIO_FORMAT_INVALID = 4294967295u,           // 0xFFFFFFFFUL
  AUDIO_FORMAT_DEFAULT = 0u,                    // 0
  AUDIO_FORMAT_PCM = 0u,                        // 0x00000000UL
  AUDIO_FORMAT_MP3 = 16777216u,                 // 0x01000000UL
  AUDIO_FORMAT_AMR_NB = 33554432u,              // 0x02000000UL
  AUDIO_FORMAT_AMR_WB = 50331648u,              // 0x03000000UL
  AUDIO_FORMAT_AAC = 67108864u,                 // 0x04000000UL
  AUDIO_FORMAT_HE_AAC_V1 = 83886080u,           // 0x05000000UL
  AUDIO_FORMAT_HE_AAC_V2 = 100663296u,          // 0x06000000UL
  AUDIO_FORMAT_VORBIS = 117440512u,             // 0x07000000UL
  AUDIO_FORMAT_OPUS = 134217728u,               // 0x08000000UL
  AUDIO_FORMAT_AC3 = 150994944u,                // 0x09000000UL
  AUDIO_FORMAT_E_AC3 = 167772160u,              // 0x0A000000UL
  AUDIO_FORMAT_DTS = 184549376u,                // 0x0B000000UL
  AUDIO_FORMAT_DTS_HD = 201326592u,             // 0x0C000000UL
  AUDIO_FORMAT_IEC61937 = 218103808u,           // 0x0D000000UL
  AUDIO_FORMAT_DOLBY_TRUEHD = 234881024u,       // 0x0E000000UL
  AUDIO_FORMAT_EVRC = 268435456u,               // 0x10000000UL
  AUDIO_FORMAT_EVRCB = 285212672u,              // 0x11000000UL
  AUDIO_FORMAT_EVRCWB = 301989888u,             // 0x12000000UL
  AUDIO_FORMAT_EVRCNW = 318767104u,             // 0x13000000UL
  AUDIO_FORMAT_AAC_ADIF = 335544320u,           // 0x14000000UL
  AUDIO_FORMAT_WMA = 352321536u,                // 0x15000000UL
  AUDIO_FORMAT_WMA_PRO = 369098752u,            // 0x16000000UL
  AUDIO_FORMAT_AMR_WB_PLUS = 385875968u,        // 0x17000000UL
  AUDIO_FORMAT_MP2 = 402653184u,                // 0x18000000UL
  AUDIO_FORMAT_QCELP = 419430400u,              // 0x19000000UL
  AUDIO_FORMAT_DSD = 436207616u,                // 0x1A000000UL
  AUDIO_FORMAT_FLAC = 452984832u,               // 0x1B000000UL
  AUDIO_FORMAT_ALAC = 469762048u,               // 0x1C000000UL
  AUDIO_FORMAT_APE = 486539264u,                // 0x1D000000UL
  AUDIO_FORMAT_AAC_ADTS = 503316480u,           // 0x1E000000UL
  AUDIO_FORMAT_SBC = 520093696u,                // 0x1F000000UL
  AUDIO_FORMAT_APTX = 536870912u,               // 0x20000000UL
  AUDIO_FORMAT_APTX_HD = 553648128u,            // 0x21000000UL
  AUDIO_FORMAT_AC4 = 570425344u,                // 0x22000000UL
  AUDIO_FORMAT_LDAC = 587202560u,               // 0x23000000UL
  AUDIO_FORMAT_MAIN_MASK = 4278190080u,         // 0xFF000000UL
  AUDIO_FORMAT_SUB_MASK = 16777215u,            // 0x00FFFFFFUL
  AUDIO_FORMAT_PCM_SUB_16_BIT = 1u,             // 0x1
  AUDIO_FORMAT_PCM_SUB_8_BIT = 2u,              // 0x2
  AUDIO_FORMAT_PCM_SUB_32_BIT = 3u,             // 0x3
  AUDIO_FORMAT_PCM_SUB_8_24_BIT = 4u,           // 0x4
  AUDIO_FORMAT_PCM_SUB_FLOAT = 5u,              // 0x5
  AUDIO_FORMAT_PCM_SUB_24_BIT_PACKED = 6u,      // 0x6
  AUDIO_FORMAT_MP3_SUB_NONE = 0u,               // 0x0
  AUDIO_FORMAT_AMR_SUB_NONE = 0u,               // 0x0
  AUDIO_FORMAT_AAC_SUB_MAIN = 1u,               // 0x1
  AUDIO_FORMAT_AAC_SUB_LC = 2u,                 // 0x2
  AUDIO_FORMAT_AAC_SUB_SSR = 4u,                // 0x4
  AUDIO_FORMAT_AAC_SUB_LTP = 8u,                // 0x8
  AUDIO_FORMAT_AAC_SUB_HE_V1 = 16u,             // 0x10
  AUDIO_FORMAT_AAC_SUB_SCALABLE = 32u,          // 0x20
  AUDIO_FORMAT_AAC_SUB_ERLC = 64u,              // 0x40
  AUDIO_FORMAT_AAC_SUB_LD = 128u,               // 0x80
  AUDIO_FORMAT_AAC_SUB_HE_V2 = 256u,            // 0x100
  AUDIO_FORMAT_AAC_SUB_ELD = 512u,              // 0x200
  AUDIO_FORMAT_VORBIS_SUB_NONE = 0u,            // 0x0
  AUDIO_FORMAT_PCM_16_BIT = 1u,                 // (PCM | PCM_SUB_16_BIT)
  AUDIO_FORMAT_PCM_8_BIT = 2u,                  // (PCM | PCM_SUB_8_BIT)
  AUDIO_FORMAT_PCM_32_BIT = 3u,                 // (PCM | PCM_SUB_32_BIT)
  AUDIO_FORMAT_PCM_8_24_BIT = 4u,               // (PCM | PCM_SUB_8_24_BIT)
  AUDIO_FORMAT_PCM_FLOAT = 5u,                  // (PCM | PCM_SUB_FLOAT)
  AUDIO_FORMAT_PCM_24_BIT_PACKED = 6u,          // (PCM | PCM_SUB_24_BIT_PACKED)
  AUDIO_FORMAT_AAC_MAIN = 67108865u,            // (AAC | AAC_SUB_MAIN)
  AUDIO_FORMAT_AAC_LC = 67108866u,              // (AAC | AAC_SUB_LC)
  AUDIO_FORMAT_AAC_SSR = 67108868u,             // (AAC | AAC_SUB_SSR)
  AUDIO_FORMAT_AAC_LTP = 67108872u,             // (AAC | AAC_SUB_LTP)
  AUDIO_FORMAT_AAC_HE_V1 = 67108880u,           // (AAC | AAC_SUB_HE_V1)
  AUDIO_FORMAT_AAC_SCALABLE = 67108896u,        // (AAC | AAC_SUB_SCALABLE)
  AUDIO_FORMAT_AAC_ERLC = 67108928u,            // (AAC | AAC_SUB_ERLC)
  AUDIO_FORMAT_AAC_LD = 67108992u,              // (AAC | AAC_SUB_LD)
  AUDIO_FORMAT_AAC_HE_V2 = 67109120u,           // (AAC | AAC_SUB_HE_V2)
  AUDIO_FORMAT_AAC_ELD = 67109376u,             // (AAC | AAC_SUB_ELD)
  AUDIO_FORMAT_AAC_ADTS_MAIN = 503316481u,      // (AAC_ADTS | AAC_SUB_MAIN)
  AUDIO_FORMAT_AAC_ADTS_LC = 503316482u,        // (AAC_ADTS | AAC_SUB_LC)
  AUDIO_FORMAT_AAC_ADTS_SSR = 503316484u,       // (AAC_ADTS | AAC_SUB_SSR)
  AUDIO_FORMAT_AAC_ADTS_LTP = 503316488u,       // (AAC_ADTS | AAC_SUB_LTP)
  AUDIO_FORMAT_AAC_ADTS_HE_V1 = 503316496u,     // (AAC_ADTS | AAC_SUB_HE_V1)
  AUDIO_FORMAT_AAC_ADTS_SCALABLE = 503316512u,  // (AAC_ADTS | AAC_SUB_SCALABLE)
  AUDIO_FORMAT_AAC_ADTS_ERLC = 503316544u,      // (AAC_ADTS | AAC_SUB_ERLC)
  AUDIO_FORMAT_AAC_ADTS_LD = 503316608u,        // (AAC_ADTS | AAC_SUB_LD)
  AUDIO_FORMAT_AAC_ADTS_HE_V2 = 503316736u,     // (AAC_ADTS | AAC_SUB_HE_V2)
  AUDIO_FORMAT_AAC_ADTS_ELD = 503316992u,       // (AAC_ADTS | AAC_SUB_ELD)
} audio_format_t;

enum {
  FCC_2 = 2,
  FCC_8 = 8,
};

enum {
  AUDIO_CHANNEL_REPRESENTATION_POSITION = 0u,      // 0
  AUDIO_CHANNEL_REPRESENTATION_INDEX = 2u,         // 2
  AUDIO_CHANNEL_NONE = 0u,                         // 0x0
  AUDIO_CHANNEL_INVALID = 3221225472u,             // 0xC0000000
  AUDIO_CHANNEL_OUT_FRONT_LEFT = 1u,               // 0x1
  AUDIO_CHANNEL_OUT_FRONT_RIGHT = 2u,              // 0x2
  AUDIO_CHANNEL_OUT_FRONT_CENTER = 4u,             // 0x4
  AUDIO_CHANNEL_OUT_LOW_FREQUENCY = 8u,            // 0x8
  AUDIO_CHANNEL_OUT_BACK_LEFT = 16u,               // 0x10
  AUDIO_CHANNEL_OUT_BACK_RIGHT = 32u,              // 0x20
  AUDIO_CHANNEL_OUT_FRONT_LEFT_OF_CENTER = 64u,    // 0x40
  AUDIO_CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 128u,  // 0x80
  AUDIO_CHANNEL_OUT_BACK_CENTER = 256u,            // 0x100
  AUDIO_CHANNEL_OUT_SIDE_LEFT = 512u,              // 0x200
  AUDIO_CHANNEL_OUT_SIDE_RIGHT = 1024u,            // 0x400
  AUDIO_CHANNEL_OUT_TOP_CENTER = 2048u,            // 0x800
  AUDIO_CHANNEL_OUT_TOP_FRONT_LEFT = 4096u,        // 0x1000
  AUDIO_CHANNEL_OUT_TOP_FRONT_CENTER = 8192u,      // 0x2000
  AUDIO_CHANNEL_OUT_TOP_FRONT_RIGHT = 16384u,      // 0x4000
  AUDIO_CHANNEL_OUT_TOP_BACK_LEFT = 32768u,        // 0x8000
  AUDIO_CHANNEL_OUT_TOP_BACK_CENTER = 65536u,      // 0x10000
  AUDIO_CHANNEL_OUT_TOP_BACK_RIGHT = 131072u,      // 0x20000
  AUDIO_CHANNEL_OUT_MONO = 1u,                     // OUT_FRONT_LEFT
  AUDIO_CHANNEL_OUT_STEREO = 3u,  // (OUT_FRONT_LEFT | OUT_FRONT_RIGHT)
  AUDIO_CHANNEL_OUT_2POINT1 =
      11u,  // ((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) | OUT_LOW_FREQUENCY)
  AUDIO_CHANNEL_OUT_QUAD = 51u,       // (((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) |
                                      // OUT_BACK_LEFT) | OUT_BACK_RIGHT)
  AUDIO_CHANNEL_OUT_QUAD_BACK = 51u,  // OUT_QUAD
  AUDIO_CHANNEL_OUT_QUAD_SIDE = 1539u,  // (((OUT_FRONT_LEFT | OUT_FRONT_RIGHT)
                                        // | OUT_SIDE_LEFT) | OUT_SIDE_RIGHT)
  AUDIO_CHANNEL_OUT_SURROUND = 263u,  // (((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) |
                                      // OUT_FRONT_CENTER) | OUT_BACK_CENTER)
  AUDIO_CHANNEL_OUT_PENTA = 55u,      // (OUT_QUAD | OUT_FRONT_CENTER)
  AUDIO_CHANNEL_OUT_5POINT1 =
      63u,  // (((((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) | OUT_FRONT_CENTER) |
            // OUT_LOW_FREQUENCY) | OUT_BACK_LEFT) | OUT_BACK_RIGHT)
  AUDIO_CHANNEL_OUT_5POINT1_BACK = 63u,  // OUT_5POINT1
  AUDIO_CHANNEL_OUT_5POINT1_SIDE =
      1551u,  // (((((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) | OUT_FRONT_CENTER) |
              // OUT_LOW_FREQUENCY) | OUT_SIDE_LEFT) | OUT_SIDE_RIGHT)
  AUDIO_CHANNEL_OUT_6POINT1 =
      319u,  // ((((((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) | OUT_FRONT_CENTER) |
             // OUT_LOW_FREQUENCY) | OUT_BACK_LEFT) | OUT_BACK_RIGHT) |
             // OUT_BACK_CENTER)
  AUDIO_CHANNEL_OUT_7POINT1 =
      1599u,  // (((((((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) | OUT_FRONT_CENTER) |
              // OUT_LOW_FREQUENCY) | OUT_BACK_LEFT) | OUT_BACK_RIGHT) |
              // OUT_SIDE_LEFT) | OUT_SIDE_RIGHT)
  AUDIO_CHANNEL_OUT_ALL =
      262143u,  // (((((((((((((((((OUT_FRONT_LEFT | OUT_FRONT_RIGHT) |
                // OUT_FRONT_CENTER) | OUT_LOW_FREQUENCY) | OUT_BACK_LEFT) |
                // OUT_BACK_RIGHT) | OUT_FRONT_LEFT_OF_CENTER) |
                // OUT_FRONT_RIGHT_OF_CENTER) | OUT_BACK_CENTER) |
                // OUT_SIDE_LEFT) | OUT_SIDE_RIGHT) | OUT_TOP_CENTER) |
                // OUT_TOP_FRONT_LEFT) | OUT_TOP_FRONT_CENTER) |
                // OUT_TOP_FRONT_RIGHT) | OUT_TOP_BACK_LEFT) |
                // OUT_TOP_BACK_CENTER) | OUT_TOP_BACK_RIGHT)
  AUDIO_CHANNEL_IN_LEFT = 4u,               // 0x4
  AUDIO_CHANNEL_IN_RIGHT = 8u,              // 0x8
  AUDIO_CHANNEL_IN_FRONT = 16u,             // 0x10
  AUDIO_CHANNEL_IN_BACK = 32u,              // 0x20
  AUDIO_CHANNEL_IN_LEFT_PROCESSED = 64u,    // 0x40
  AUDIO_CHANNEL_IN_RIGHT_PROCESSED = 128u,  // 0x80
  AUDIO_CHANNEL_IN_FRONT_PROCESSED = 256u,  // 0x100
  AUDIO_CHANNEL_IN_BACK_PROCESSED = 512u,   // 0x200
  AUDIO_CHANNEL_IN_PRESSURE = 1024u,        // 0x400
  AUDIO_CHANNEL_IN_X_AXIS = 2048u,          // 0x800
  AUDIO_CHANNEL_IN_Y_AXIS = 4096u,          // 0x1000
  AUDIO_CHANNEL_IN_Z_AXIS = 8192u,          // 0x2000
  AUDIO_CHANNEL_IN_VOICE_UPLINK = 16384u,   // 0x4000
  AUDIO_CHANNEL_IN_VOICE_DNLINK = 32768u,   // 0x8000
  AUDIO_CHANNEL_IN_MONO = 16u,              // IN_FRONT
  AUDIO_CHANNEL_IN_STEREO = 12u,            // (IN_LEFT | IN_RIGHT)
  AUDIO_CHANNEL_IN_FRONT_BACK = 48u,        // (IN_FRONT | IN_BACK)
  AUDIO_CHANNEL_IN_6 = 252u,  // (((((IN_LEFT | IN_RIGHT) | IN_FRONT) | IN_BACK)
                              // | IN_LEFT_PROCESSED) | IN_RIGHT_PROCESSED)
  AUDIO_CHANNEL_IN_VOICE_UPLINK_MONO = 16400u,  // (IN_VOICE_UPLINK | IN_MONO)
  AUDIO_CHANNEL_IN_VOICE_DNLINK_MONO = 32784u,  // (IN_VOICE_DNLINK | IN_MONO)
  AUDIO_CHANNEL_IN_VOICE_CALL_MONO =
      49168u,  // (IN_VOICE_UPLINK_MONO | IN_VOICE_DNLINK_MONO)
  AUDIO_CHANNEL_IN_ALL =
      65532u,  // (((((((((((((IN_LEFT | IN_RIGHT) | IN_FRONT) | IN_BACK) |
               // IN_LEFT_PROCESSED) | IN_RIGHT_PROCESSED) | IN_FRONT_PROCESSED)
               // | IN_BACK_PROCESSED) | IN_PRESSURE) | IN_X_AXIS) | IN_Y_AXIS)
               // | IN_Z_AXIS) | IN_VOICE_UPLINK) | IN_VOICE_DNLINK)
  AUDIO_CHANNEL_COUNT_MAX = 30u,          // 30
  AUDIO_CHANNEL_INDEX_HDR = 2147483648u,  // (REPRESENTATION_INDEX << COUNT_MAX)
  AUDIO_CHANNEL_INDEX_MASK_1 = 2147483649u,  // (INDEX_HDR | ((1 << 1) - 1))
  AUDIO_CHANNEL_INDEX_MASK_2 = 2147483651u,  // (INDEX_HDR | ((1 << 2) - 1))
  AUDIO_CHANNEL_INDEX_MASK_3 = 2147483655u,  // (INDEX_HDR | ((1 << 3) - 1))
  AUDIO_CHANNEL_INDEX_MASK_4 = 2147483663u,  // (INDEX_HDR | ((1 << 4) - 1))
  AUDIO_CHANNEL_INDEX_MASK_5 = 2147483679u,  // (INDEX_HDR | ((1 << 5) - 1))
  AUDIO_CHANNEL_INDEX_MASK_6 = 2147483711u,  // (INDEX_HDR | ((1 << 6) - 1))
  AUDIO_CHANNEL_INDEX_MASK_7 = 2147483775u,  // (INDEX_HDR | ((1 << 7) - 1))
  AUDIO_CHANNEL_INDEX_MASK_8 = 2147483903u,  // (INDEX_HDR | ((1 << 8) - 1))
};

enum {
  AUDIO_INTERLEAVE_LEFT = 0,
  AUDIO_INTERLEAVE_RIGHT = 1,
};

typedef enum {
  AUDIO_MODE_INVALID = -2,  // (-2)
  AUDIO_MODE_CURRENT = -1,  // (-1)
  AUDIO_MODE_NORMAL = 0,
  AUDIO_MODE_RINGTONE = 1,
  AUDIO_MODE_IN_CALL = 2,
  AUDIO_MODE_IN_COMMUNICATION = 3,
  AUDIO_MODE_CNT = 4,
  AUDIO_MODE_MAX = 3,  // (CNT - 1)
} audio_mode_t;

enum {
  AUDIO_DEVICE_NONE = 0u,                             // 0x0
  AUDIO_DEVICE_BIT_IN = 2147483648u,                  // 0x80000000
  AUDIO_DEVICE_BIT_DEFAULT = 1073741824u,             // 0x40000000
  AUDIO_DEVICE_OUT_EARPIECE = 1u,                     // 0x1
  AUDIO_DEVICE_OUT_SPEAKER = 2u,                      // 0x2
  AUDIO_DEVICE_OUT_WIRED_HEADSET = 4u,                // 0x4
  AUDIO_DEVICE_OUT_WIRED_HEADPHONE = 8u,              // 0x8
  AUDIO_DEVICE_OUT_BLUETOOTH_SCO = 16u,               // 0x10
  AUDIO_DEVICE_OUT_BLUETOOTH_SCO_HEADSET = 32u,       // 0x20
  AUDIO_DEVICE_OUT_BLUETOOTH_SCO_CARKIT = 64u,        // 0x40
  AUDIO_DEVICE_OUT_BLUETOOTH_A2DP = 128u,             // 0x80
  AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES = 256u,  // 0x100
  AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER = 512u,     // 0x200
  AUDIO_DEVICE_OUT_AUX_DIGITAL = 1024u,               // 0x400
  AUDIO_DEVICE_OUT_HDMI = 1024u,                      // OUT_AUX_DIGITAL
  AUDIO_DEVICE_OUT_ANLG_DOCK_HEADSET = 2048u,         // 0x800
  AUDIO_DEVICE_OUT_DGTL_DOCK_HEADSET = 4096u,         // 0x1000
  AUDIO_DEVICE_OUT_USB_ACCESSORY = 8192u,             // 0x2000
  AUDIO_DEVICE_OUT_USB_DEVICE = 16384u,               // 0x4000
  AUDIO_DEVICE_OUT_REMOTE_SUBMIX = 32768u,            // 0x8000
  AUDIO_DEVICE_OUT_TELEPHONY_TX = 65536u,             // 0x10000
  AUDIO_DEVICE_OUT_LINE = 131072u,                    // 0x20000
  AUDIO_DEVICE_OUT_HDMI_ARC = 262144u,                // 0x40000
  AUDIO_DEVICE_OUT_SPDIF = 524288u,                   // 0x80000
  AUDIO_DEVICE_OUT_FM = 1048576u,                     // 0x100000
  AUDIO_DEVICE_OUT_AUX_LINE = 2097152u,               // 0x200000
  AUDIO_DEVICE_OUT_SPEAKER_SAFE = 4194304u,           // 0x400000
  AUDIO_DEVICE_OUT_IP = 8388608u,                     // 0x800000
  AUDIO_DEVICE_OUT_BUS = 16777216u,                   // 0x1000000
  AUDIO_DEVICE_OUT_PROXY = 33554432u,                 // 0x2000000
  AUDIO_DEVICE_OUT_USB_HEADSET = 67108864u,           // 0x4000000
#ifdef PRODUCT_MANUFACTURER_SPRD
  AUDIO_DEVICE_OUT_FM_HEADSET = 268435456u,           // 0x10000000
  AUDIO_DEVICE_OUT_FM_SPEAKER = 536870912u,           // 0x20000000
#endif
  AUDIO_DEVICE_OUT_DEFAULT = 1073741824u,             // BIT_DEFAULT
  AUDIO_DEVICE_OUT_ALL =
      1207959551u,  // (((((((((((((((((((((((((((OUT_EARPIECE | OUT_SPEAKER) |
                    // OUT_WIRED_HEADSET) | OUT_WIRED_HEADPHONE) |
                    // OUT_BLUETOOTH_SCO) | OUT_BLUETOOTH_SCO_HEADSET) |
                    // OUT_BLUETOOTH_SCO_CARKIT) | OUT_BLUETOOTH_A2DP) |
                    // OUT_BLUETOOTH_A2DP_HEADPHONES) |
                    // OUT_BLUETOOTH_A2DP_SPEAKER) | OUT_HDMI) |
                    // OUT_ANLG_DOCK_HEADSET) | OUT_DGTL_DOCK_HEADSET) |
                    // OUT_USB_ACCESSORY) | OUT_USB_DEVICE) | OUT_REMOTE_SUBMIX)
                    // | OUT_TELEPHONY_TX) | OUT_LINE) | OUT_HDMI_ARC) |
                    // OUT_SPDIF) | OUT_FM) | OUT_AUX_LINE) | OUT_SPEAKER_SAFE)
                    // | OUT_IP) | OUT_BUS) | OUT_PROXY) | OUT_USB_HEADSET) |
                    // OUT_DEFAULT)
  AUDIO_DEVICE_OUT_ALL_A2DP =
      896u,  // ((OUT_BLUETOOTH_A2DP | OUT_BLUETOOTH_A2DP_HEADPHONES) |
             // OUT_BLUETOOTH_A2DP_SPEAKER)
  AUDIO_DEVICE_OUT_ALL_SCO =
      112u,  // ((OUT_BLUETOOTH_SCO | OUT_BLUETOOTH_SCO_HEADSET) |
             // OUT_BLUETOOTH_SCO_CARKIT)
  AUDIO_DEVICE_OUT_ALL_USB =
      67133440u,  // ((OUT_USB_ACCESSORY | OUT_USB_DEVICE) | OUT_USB_HEADSET)
  AUDIO_DEVICE_IN_COMMUNICATION = 2147483649u,          // (BIT_IN | 0x1)
  AUDIO_DEVICE_IN_AMBIENT = 2147483650u,                // (BIT_IN | 0x2)
  AUDIO_DEVICE_IN_BUILTIN_MIC = 2147483652u,            // (BIT_IN | 0x4)
  AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET = 2147483656u,  // (BIT_IN | 0x8)
  AUDIO_DEVICE_IN_WIRED_HEADSET = 2147483664u,          // (BIT_IN | 0x10)
  AUDIO_DEVICE_IN_AUX_DIGITAL = 2147483680u,            // (BIT_IN | 0x20)
  AUDIO_DEVICE_IN_HDMI = 2147483680u,                   // IN_AUX_DIGITAL
  AUDIO_DEVICE_IN_VOICE_CALL = 2147483712u,             // (BIT_IN | 0x40)
  AUDIO_DEVICE_IN_TELEPHONY_RX = 2147483712u,           // IN_VOICE_CALL
  AUDIO_DEVICE_IN_BACK_MIC = 2147483776u,               // (BIT_IN | 0x80)
  AUDIO_DEVICE_IN_REMOTE_SUBMIX = 2147483904u,          // (BIT_IN | 0x100)
  AUDIO_DEVICE_IN_ANLG_DOCK_HEADSET = 2147484160u,      // (BIT_IN | 0x200)
  AUDIO_DEVICE_IN_DGTL_DOCK_HEADSET = 2147484672u,      // (BIT_IN | 0x400)
  AUDIO_DEVICE_IN_USB_ACCESSORY = 2147485696u,          // (BIT_IN | 0x800)
  AUDIO_DEVICE_IN_USB_DEVICE = 2147487744u,             // (BIT_IN | 0x1000)
  AUDIO_DEVICE_IN_FM_TUNER = 2147491840u,               // (BIT_IN | 0x2000)
  AUDIO_DEVICE_IN_TV_TUNER = 2147500032u,               // (BIT_IN | 0x4000)
  AUDIO_DEVICE_IN_LINE = 2147516416u,                   // (BIT_IN | 0x8000)
  AUDIO_DEVICE_IN_SPDIF = 2147549184u,                  // (BIT_IN | 0x10000)
  AUDIO_DEVICE_IN_BLUETOOTH_A2DP = 2147614720u,         // (BIT_IN | 0x20000)
  AUDIO_DEVICE_IN_LOOPBACK = 2147745792u,               // (BIT_IN | 0x40000)
  AUDIO_DEVICE_IN_IP = 2148007936u,                     // (BIT_IN | 0x80000)
  AUDIO_DEVICE_IN_BUS = 2148532224u,                    // (BIT_IN | 0x100000)
  AUDIO_DEVICE_IN_PROXY = 2164260864u,                  // (BIT_IN | 0x1000000)
  AUDIO_DEVICE_IN_USB_HEADSET = 2181038080u,            // (BIT_IN | 0x2000000)
  AUDIO_DEVICE_IN_DEFAULT = 3221225472u,  // (BIT_IN | BIT_DEFAULT)
  AUDIO_DEVICE_IN_ALL =
      3273654271u,  // (((((((((((((((((((((((IN_COMMUNICATION | IN_AMBIENT) |
                    // IN_BUILTIN_MIC) | IN_BLUETOOTH_SCO_HEADSET) |
                    // IN_WIRED_HEADSET) | IN_HDMI) | IN_TELEPHONY_RX) |
                    // IN_BACK_MIC) | IN_REMOTE_SUBMIX) | IN_ANLG_DOCK_HEADSET)
                    // | IN_DGTL_DOCK_HEADSET) | IN_USB_ACCESSORY) |
                    // IN_USB_DEVICE) | IN_FM_TUNER) | IN_TV_TUNER) | IN_LINE) |
                    // IN_SPDIF) | IN_BLUETOOTH_A2DP) | IN_LOOPBACK) | IN_IP) |
                    // IN_BUS) | IN_PROXY) | IN_USB_HEADSET) | IN_DEFAULT)
  AUDIO_DEVICE_IN_ALL_SCO = 2147483656u,  // IN_BLUETOOTH_SCO_HEADSET
  AUDIO_DEVICE_IN_ALL_USB =
      2181044224u,  // ((IN_USB_ACCESSORY | IN_USB_DEVICE) | IN_USB_HEADSET)
};

typedef enum {
  AUDIO_OUTPUT_FLAG_NONE = 0,                // 0x0
  AUDIO_OUTPUT_FLAG_DIRECT = 1,              // 0x1
  AUDIO_OUTPUT_FLAG_PRIMARY = 2,             // 0x2
  AUDIO_OUTPUT_FLAG_FAST = 4,                // 0x4
  AUDIO_OUTPUT_FLAG_DEEP_BUFFER = 8,         // 0x8
  AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD = 16,   // 0x10
  AUDIO_OUTPUT_FLAG_NON_BLOCKING = 32,       // 0x20
  AUDIO_OUTPUT_FLAG_HW_AV_SYNC = 64,         // 0x40
  AUDIO_OUTPUT_FLAG_TTS = 128,               // 0x80
  AUDIO_OUTPUT_FLAG_RAW = 256,               // 0x100
  AUDIO_OUTPUT_FLAG_SYNC = 512,              // 0x200
  AUDIO_OUTPUT_FLAG_IEC958_NONAUDIO = 1024,  // 0x400
  AUDIO_OUTPUT_FLAG_DIRECT_PCM = 8192,       // 0x2000
  AUDIO_OUTPUT_FLAG_MMAP_NOIRQ = 16384,      // 0x4000
  AUDIO_OUTPUT_FLAG_VOIP_RX = 32768,         // 0x8000
} audio_output_flags_t;

typedef enum {
  AUDIO_INPUT_FLAG_NONE = 0,         // 0x0
  AUDIO_INPUT_FLAG_FAST = 1,         // 0x1
  AUDIO_INPUT_FLAG_HW_HOTWORD = 2,   // 0x2
  AUDIO_INPUT_FLAG_RAW = 4,          // 0x4
  AUDIO_INPUT_FLAG_SYNC = 8,         // 0x8
  AUDIO_INPUT_FLAG_MMAP_NOIRQ = 16,  // 0x10
  AUDIO_INPUT_FLAG_VOIP_TX = 32,     // 0x20
} audio_input_flags_t;

typedef enum {
  AUDIO_USAGE_UNKNOWN = 0,
  AUDIO_USAGE_MEDIA = 1,
  AUDIO_USAGE_VOICE_COMMUNICATION = 2,
  AUDIO_USAGE_VOICE_COMMUNICATION_SIGNALLING = 3,
  AUDIO_USAGE_ALARM = 4,
  AUDIO_USAGE_NOTIFICATION = 5,
  AUDIO_USAGE_NOTIFICATION_TELEPHONY_RINGTONE = 6,
  AUDIO_USAGE_NOTIFICATION_COMMUNICATION_REQUEST = 7,
  AUDIO_USAGE_NOTIFICATION_COMMUNICATION_INSTANT = 8,
  AUDIO_USAGE_NOTIFICATION_COMMUNICATION_DELAYED = 9,
  AUDIO_USAGE_NOTIFICATION_EVENT = 10,
  AUDIO_USAGE_ASSISTANCE_ACCESSIBILITY = 11,
  AUDIO_USAGE_ASSISTANCE_NAVIGATION_GUIDANCE = 12,
  AUDIO_USAGE_ASSISTANCE_SONIFICATION = 13,
  AUDIO_USAGE_GAME = 14,
  AUDIO_USAGE_VIRTUAL_SOURCE = 15,
  AUDIO_USAGE_ASSISTANT = 16,
  AUDIO_USAGE_CNT = 17,
  AUDIO_USAGE_MAX = 16,  // (CNT - 1)
} audio_usage_t;

enum {
  AUDIO_GAIN_MODE_JOINT = 1u,     // 0x1
  AUDIO_GAIN_MODE_CHANNELS = 2u,  // 0x2
  AUDIO_GAIN_MODE_RAMP = 4u,      // 0x4
};

typedef enum {
  AUDIO_PORT_ROLE_NONE = 0,
  AUDIO_PORT_ROLE_SOURCE = 1,
  AUDIO_PORT_ROLE_SINK = 2,
} audio_port_role_t;

typedef enum {
  AUDIO_PORT_TYPE_NONE = 0,
  AUDIO_PORT_TYPE_DEVICE = 1,
  AUDIO_PORT_TYPE_MIX = 2,
  AUDIO_PORT_TYPE_SESSION = 3,
} audio_port_type_t;

enum {
  AUDIO_PORT_CONFIG_SAMPLE_RATE = 1u,   // 0x1
  AUDIO_PORT_CONFIG_CHANNEL_MASK = 2u,  // 0x2
  AUDIO_PORT_CONFIG_FORMAT = 4u,        // 0x4
  AUDIO_PORT_CONFIG_GAIN = 8u,          // 0x8
  AUDIO_PORT_CONFIG_ALL =
      15u,  // (((SAMPLE_RATE | CHANNEL_MASK) | FORMAT) | GAIN)
};

typedef enum {
  AUDIO_LATENCY_LOW = 0,
  AUDIO_LATENCY_NORMAL = 1,
} audio_mix_latency_class_t;

#ifdef __cplusplus
}
#endif

#endif  // HIDL_GENERATED_ANDROID_HARDWARE_AUDIO_COMMON_V2_0_EXPORTED_CONSTANTS_H_
