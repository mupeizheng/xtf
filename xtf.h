#ifndef XTF_H
#define XTF_H

#include <cstdint>

//本数据结构根据 （XTF）Rev.42文档，通过AI生成，数据并不一定完全正确，注意辨别！！！！！！！！！

#pragma pack(push, 1)  // 确保1字节对齐

enum TypeOfChannel : uint8_t {
    CHAN_SUBBOTTOM = 0,     // 海底剖面数据
    CHAN_PORT = 1,          // 左舷侧扫通道
    CHAN_STBD = 2,          // 右舷侧扫通道
    CHAN_BATHYMETRY = 3,    // 测深数据通道
};

struct CHANINFO {
    // ===== 通道基础信息 =====
    uint8_t  TypeOfChannel;   // 通道类型 (字节0)
    // Comment列明确值:
    // 0=SUBBOTTOM,1=PORT,2=STBD,3=BATHYMETRY
    uint8_t  SubChannelNumber; // 子通道号 (字节1)
    uint16_t CorrectionFlags;  // 校正标志 (字节2-3)
    // Comment: "1=slant-range,2=ground range"
    uint16_t UniPolar;         // 极性 (字节4-5)
    // Comment: "0=polar,1=unipolar"
    uint16_t BytesPerSample;   // 每样本字节数 (字节6-7)
    // Comment: "1=8-bit,2=16-bit,4=32-bit"
    uint32_t Reserved;         // 保留 (字节8-11)
    char     ChannelName[16];  // 通道名称 (字节12-27)

    // ===== 声学参数 =====
    float    VoltScale;        // 电压比例 (字节28-31)
    float    Frequency;        // 频率(Hz) (字节32-35)
    float    HorizBeamAngle;   // 水平波束角(°) (字节36-39)
    float    TiltAngle;        // 倾斜角(°) (字节40-43)
    float    BeamWidth;        // 波束宽度(°) (字节44-47)

    // ===== 位置偏移 =====
    float    OffsetX;          // X偏移(右舷为正) (字节48-51)
    float    OffsetY;          // Y偏移(船头为正) (字节52-55)
    float    OffsetZ;          // Z偏移(向下为正) (字节56-59)
    float    OffsetYaw;        // 偏航偏移(°) (字节60-63)
    float    OffsetPitch;      // 俯仰偏移(°) (字节64-67)
    float    OffsetRoll;       // 横滚偏移(°) (字节68-71)

    // ===== 阵列配置 =====
    uint16_t BeamsPerArray;    // 每阵列波束数 (字节72-73)

    // ===== 保留区域 =====
    uint8_t  ReservedArea2[54]; // 保留 (字节74-127)
};



enum SonarType : uint16_t {
    SONAR_NONE = 0,               // 未指定（默认值）
    SONAR_JAMSTEC = 1,            // JAMSTEC海底剖面仪（2通道Chirp系统）
    SONAR_ANALOG_C31 = 2,         // PC31 8通道模拟系统
    SONAR_SIS1000 = 3,            // SIS-1000 Chirp声纳
    SONAR_ANALOG_32CHAN = 4,      // 32通道DSP频谱卡系统
    SONAR_KLEIN2000 = 5,          // Klein 2000数字接口系统
    SONAR_RWS = 6,                // 带特殊导航代码的PC31模拟系统
    SONAR_DF1000 = 7,             // EG&G DF1000数字接口
    SONAR_SEABAT = 8,             // Reson SeaBat 900x模拟/串行系统
    SONAR_KLEIN595 = 9,           // Klein 595 4通道系统
    SONAR_EGG260 = 10,            // EG&G 260 2通道系统
    SONAR_SONATECH_DDS = 11,      // Sonatech潜水员探测系统
    SONAR_ECHOSCAN = 12,          // Odom EchoScanII多波束系统（带同步侧扫）
    SONAR_ELAC = 13,              // Elac多波束系统
    SONAR_KLEIN5000 = 14,         // Klein 5000数字接口系统
    SONAR_SEABAT_8101 = 15,       // Reson SeaBat 8101
    SONAR_IMAGENEX_858 = 16,      // Imagenex 858型声纳
    SONAR_USN_SILOS = 17,         // USN SILOS 3通道模拟系统
    SONAR_SONATECH_SHR = 18,      // Sonatech超高分辨率侧扫声纳
    SONAR_DELPH_AU32 = 19,        // Delph AU32模拟输入(2通道)
    SONAR_GENERIC = 20,           // 通用内存映射文件接口声纳
    SONAR_SIMRAD_SM2000 = 21,     // Simrad SM2000多波束测深系统
    SONAR_MM_STEREO = 22,         // 标准多媒体音频
    SONAR_EDGETECH_ACI = 23,      // Edgetech(EG&G) ACI卡(用于PC31卡的260声纳)
    SONAR_EDGETECH_BLACKBOX = 24, // Edgetech黑盒系统
    SONAR_FUGRO_DEEPTOW = 25,     // Fugro深拖系统
    SONAR_CC_EDGETECH_CHIRP = 26, // C&C Edgetech Chirp转换程序
    SONAR_DTI_SAS = 27,            // DTI SAS合成孔径处理器
    SONAR_FUGRO_OSIRIS_SS = 28,    // Fugro Osiris AUV侧扫数据
    SONAR_FUGRO_OSIRIS_MB = 29,    // Fugro Osiris AUV多波束数据
    SONAR_GEOACOUSTICS_SLS = 30,  // Geoacoustics SLS系统
    SONAR_SIMRAD_EM = 31,         // Simrad EM2000/EM3000系统
    SONAR_KLEIN3000 = 32,         // Klein 3000系统
    SONAR_SHRSSS_CHIRP = 33,      // SHRSSS Chirp系统
    SONAR_BENTHOS_C3D = 34,       // Benthos C3D SARA/CAATI
    SONAR_EDGETECH_MPX = 35,      // Edgetech MP-X
    SONAR_CMAX = 36,              // C-MAX系统
    SONAR_BENTHOS_SIS1624 = 37,   // Benthos SIS1624
    SONAR_EDGETECH_4200 = 38,     // Edgetech 4200
    SONAR_BENTHOS_SIS1500 = 39,   // Benthos SIS1500
    SONAR_BENTHOS_SIS1502 = 40,   // Benthos SIS1502
    SONAR_BENTHOS_SIS3000 = 41,   // Benthos SIS3000
    SONAR_BENTHOS_SIS7000 = 42,   // Benthos SIS7000
    SONAR_DF1000_DCU = 43,        // DF1000 DCU
    SONAR_NONE_SIDESCAN = 44,     // 无侧扫（特殊标识）
    SONAR_NONE_MULTIBEAM = 45,    // 无多波束（特殊标识）
    SONAR_RESON_7125 = 46,        // Reson 7125
    SONAR_CODA_ECHOSCOPE = 47,    // CODA Echoscope
    SONAR_KONGSBERG_SAS = 48,     // Kongsberg SAS
    SONAR_QINSY = 49,             // QINSy系统
    SONAR_GEOACOUSTICS_DSSS = 50, // GeoAcoustics DSSS
    SONAR_CMAX_USB = 51,          // CMAX USB接口系统
    SONAR_SWATHPLUS_BATHY = 52,   // SwathPlus测深系统
    SONAR_R2SONIC_QINSY = 53,     // R2Sonic QINSy
    SONAR_SWATHPLUS_CONVERTED = 54, // 转换的SwathPlus测深数据
    SONAR_R2SONIC_TRITON = 55,    // R2Sonic Triton
    SONAR_EDGETECH_4600 = 56,     // Edgetech 4600
    SONAR_KLEIN_3500 = 57,        // Klein 3500
    SONAR_KLEIN_5900 = 58         // Klein 5900

};

struct XTFFILEHEADER {
    // ===== 文件头标识 =====
    uint8_t  FileFormat;       // 必须为0x7B (字节0)
    uint8_t  SystemType;       // 必须为1 (字节1)
    char     RecordingProgramName[8];  // 记录程序名 (字节2-9)
    char     RecordingProgramVersion[8]; // 版本号 (字节10-17)

    // ===== 设备信息 =====
    char     SonarName[16];    // 声纳服务器名称 (字节18-33)
    uint16_t SonarType;        // 声纳类型 (字节34-35)
    // Comment列明确值:
    // 0=NONE,1=JAMSTEC,2=ANALOG_C31...58=KLEIN_5900
    char     NoteString[64];   // 注释文本 (字节36-99)
    char     ThisFileName[64]; // 本文件名 (字节100-163)

    // ===== 通道配置 =====
    uint16_t NavUnits;         // 导航单位 (字节164-165)
    // Comment: "0=Meters,3=Lat/Long"
    uint16_t NumberOfSonarChannels;  // 声纳通道数 (字节166-167)
    uint16_t NumberOfBathymetryChannels; // 测深通道数 (字节168-169)
    uint8_t  NumberOfSnippetChannels;    // 片段通道数 (字节170)
    uint8_t  NumberOfForwardLookArrays;  // 前视阵列数 (字节171)
    uint16_t NumberOfEchoStrengthChannels; // 回波强度通道数 (字节172-173)
    uint8_t  NumberOfInterferometryChannels; // 干涉通道数 (字节174)
    uint8_t  Reserved1;        // 保留 (必须置0) (字节175)
    uint16_t Reserved2;        // 保留 (必须置0) (字节176-177)

    // ===== 参考系参数 =====
    float    ReferencePointHeight; // 参考点高度(m) (字节178-181)
    uint8_t  ProjectionType[12];   // 未使用 (必须置0) (字节182-193)
    uint8_t  SpheriodType[10];     // 未使用 (必须置0) (字节194-203)
    int32_t  NavigationLatency;    // 导航延迟(ms) (字节204-207)
    float    OriginY;              // 未使用 (必须置0) (字节208-211)
    float    OriginX;              // 未使用 (必须置0) (字节212-215)
    float    NavOffsetY;           // 导航Y偏移(m) (字节216-219)
    float    NavOffsetX;           // 导航X偏移(m) (字节220-223)
    float    NavOffsetZ;           // 导航Z偏移(m) (字节224-227)
    float    NavOffsetYaw;         // 导航偏航偏移(°) (字节228-231)
    float    MRUOffsetY;           // MRU Y偏移(m) (字节232-235)
    float    MRUOffsetX;           // MRU X偏移(m) (字节236-239)
    float    MRUOffsetZ;           // MRU Z偏移(m) (字节240-243)
    float    MRUOffsetYaw;         // MRU偏航偏移(°) (字节244-247)
    float    MRUOffsetPitch;       // MRU俯仰偏移(°) (字节248-251)
    float    MRUOffsetRoll;        // MRU横滚偏移(°) (字节252-255)

    // ===== 通道信息数组 =====
    CHANINFO ChanInfo[6];          // 通道信息 (字节256-1023)
    // 如果NumberOfSonarChannels>6，需扩展头
};


// 声纳通道头（每个数据包前都有）
struct XTFCHANHEADER {
    uint16_t MagicNumber;
    uint8_t  HeaderType;
    uint8_t  SubChannelNumber;
    uint16_t NumChansToFollow;
    uint16_t Reserved1[2];
    uint32_t NumBytesThisRecord;
};




struct XTFAttitudeData {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;          // 必须设置为0xFACE (字节0-1)
    uint8_t HeaderType;            // 必须设置为3 (XTF_HEADER_ATTITUDE) (字节2)
    uint8_t SubChannelNumber;
    uint16_t NumChansToFollow;     // 后续通道数 (通常为0) (字节4-5)
    uint16_t Reserved1[2];          // 保留 (设置为0) (字节6-9)
    uint32_t NumBytesThisRecord;   // 数据包总字节数 (字节10-13)
    uint32_t Reserved2[2];          // 保留 (设置为0) (字节14-21)

    // ===== 时间信息 =====
    uint32_t EpochMicroseconds;    // 微秒 (0-999999) (字节22-25)
    uint32_t SourceEpoch;          // 1970年以来的秒数 (字节26-29)

    // ===== 姿态数据 =====
    float Pitch;                   // 俯仰角 (正=船头上扬) (字节30-33)
    float Roll;                    // 横滚角 (正=向右倾斜) (字节34-37)
    float Heave;                   // 升沉 (正=传感器上升) (字节38-41)
    float Yaw;                     // 偏航角 (正=向右转) (字节42-45)

    // ===== 导航信息 =====
    uint32_t TimeTag;              // 系统时间参考 (毫秒) (字节46-49)
    float Heading;                 // 航向角 (度) (字节50-53)

    // ===== 时间戳 =====
    uint16_t Year;                 // 年 (字节54-55)
    uint8_t Month;                 // 月 (字节56)
    uint8_t Day;                   // 日 (字节57)
    uint8_t Hour;                  // 时 (字节58)
    uint8_t Minutes;               // 分 (字节59)
    uint8_t Seconds;               // 秒 (字节60)
    uint16_t Milliseconds;         // 毫秒 (0-999) (字节61-62)

    // ===== 扩展字段 =====
    uint8_t Reserved3;             // 保留 (设置为0) (字节63)


};

// 注释类型枚举（根据SubChannelNumber定义）
enum NoteType : uint8_t {
    NOTE_GENERAL = 0,      // 通用注释（来自参数窗口）
    NOTE_VESSEL_NAME = 1,  // 船只名称
    NOTE_SURVEY_AREA = 2,  // 调查区域
    NOTE_OPERATOR = 3,     // 操作员名称
};

struct XTFNOTESHEADER {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须设置为0xFACE（标识XTF文件）
    uint8_t  HeaderType;       // 必须设置为1（XTF_HEADER_NOTES）
    uint8_t  SubChannelNumber; // 注释类型（见NoteType枚举）

    uint16_t NumChansToFollow; // 未使用（设置为0）
    uint16_t Reserved[2];      // 保留字段（设置为0）

    uint32_t NumBytesThisRecord; // 必须设置为256（固定包大小）

    // ===== 时间戳 =====
    uint16_t Year;             // 注释创建年份
    uint8_t  Month;            // 注释创建月份（1-12）
    uint8_t  Day;              // 注释创建日期（1-31）
    uint8_t  Hour;             // 注释创建小时（0-23）
    uint8_t  Minute;           // 注释创建分钟（0-59）
    uint8_t  Second;           // 注释创建秒（0-59）

    // ===== 注释内容 =====
    uint8_t  ReservedBytes[35]; // 保留字段（设置为0）
    char     NotesText[200];    // UTF-8编码的注释文本（非null-terminated）
};



struct XTFRAWSERIALHEADER {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须设置为0xFACE（标识XTF文件）(字节0-1)
    uint8_t  HeaderType;       // 必须设置为6（XTF_HEADER_RAW_SERIAL）(字节2)
    uint8_t  SerialPort;       // 串口编号：COM1=1, COM2=2,... (字节3)
    uint16_t NumChansToFollow; // 未使用（设置为0）(字节4-5)
    uint16_t Reserved[2];      // 保留字段（设置为0）(字节6-9)
    uint32_t NumBytesThisRecord; // 数据包总字节数（含包头）(字节10-13)

    // ===== 时间戳 =====
    uint16_t Year;             // 年（字节14-15）
    uint8_t  Month;            // 月（1-12）（字节16）
    uint8_t  Day;              // 日（1-31）（字节17）
    uint8_t  Hour;             // 时（0-23）（字节18）
    uint8_t  Minute;           // 分（0-59）（字节19）
    uint8_t  Second;           // 秒（0-59）（字节20）
    uint8_t  HSeconds;         // 百分之一秒（0-99）（字节21）
    uint16_t JulianDay;        // 自1月1日以来的天数（字节22-23）

    // ===== 数据内容 =====
    uint32_t TimeTag;          // 系统毫秒计时值（字节24-27）
    uint16_t StringSize;       // 有效ASCII数据长度（字节28-29）
    //char     RawAsciiData[StringSize];  // 可变长原始ASCII数据（实际长度=StringSize）(字节30-...)
};




enum XTFPINGHeaderType : uint16_t {  // 注意：实际为uint8_t，但范围覆盖0-199
    // ===== 基础数据类型 =====
    XTF_HEADER_SONAR                = 0,   // 侧扫声纳数据
    XTF_HEADER_NOTES                = 1,   // 注释
    XTF_HEADER_BATHY                = 2,   // 原始测深数据
    XTF_HEADER_ATTITUDE             = 3,   // 姿态数据
    XTF_HEADER_FORWARD              = 4,   // 前视声纳
    XTF_HEADER_ELAC                 = 5,   // ELAC数据
    XTF_HEADER_RAW_SERIAL           = 6,   // 原始串口数据
    XTF_HEADER_EMBED_HEAD           = 7,   // 嵌入头
    XTF_HEADER_HIDDEN_SONAR         = 8,   // Klein冗余ping
    XTF_HEADER_SEAVIEW_PROCESSED_BATHY = 9,  // Seaview处理后的测深
    XTF_HEADER_SEAVIEW_DEPTHS       = 10,  // Seaview深度数据

    // ===== 传感器数据 =====
    XTF_HEADER_RSVD_HIGHSPEED_SENSOR = 11,  // 高速传感器（Klein）
    XTF_HEADER_ECHOSTRENGTH         = 12,  // 回波强度（ELAC）
    XTF_HEADER_GEOREC               = 13,  // 地理记录
    XTF_HEADER_KLEIN_RAW_BATHY      = 14,  // Klein原始测深
    XTF_HEADER_HIGHSPEED_SENSOR2    = 15,  // 高速传感器2（Klein）
    XTF_HEADER_ELAC_XSE             = 16,  // ELAC双头数据
    XTF_HEADER_BATHY_XYZA           = 17,  // 处理后的测深数据
    XTF_HEADER_K5000_BATHY_IQ       = 18,  // Klein 5000 IQ数据
    XTF_HEADER_BATHY_SNIPPET        = 19,  // 测深片段
    XTF_HEADER_GPS                  = 20,  // GPS定位
    XTF_HEADER_STAT                 = 21,  // 统计信息
    XTF_HEADER_SINGLEBEAM           = 22,  // 单波束
    XTF_HEADER_GYRO                 = 23,  // 陀螺仪
    XTF_HEADER_TRACKPOINT           = 24,  // 跟踪点
    XTF_HEADER_MULTIBEAM            = 25,  // 多波束
    XTF_HEADER_Q_SINGLEBEAM         = 26,  // QPS单波束
    XTF_HEADER_Q_MULTITX            = 27,  // QPS多换能器
    XTF_HEADER_Q_MULTIBEAM          = 28,  // QPS多波束

    // ===== 特殊类型 =====
    XTF_HEADER_TIME                 = 50,  // 时间同步
    XTF_HEADER_BENTHOS_CAATI_SARA   = 60,  // Benthos CAATI/SARA数据
    XTF_HEADER_7125                 = 61,  // Reson 7125数据
    XTF_HEADER_7125_SNIPPET         = 62,  // Reson 7125片段
    XTF_HEADER_QINSY_R2SONIC_BATHY  = 65,  // QINSy R2Sonic测深
    XTF_HEADER_QINSY_R2SONIC_FTS    = 66,  // QINSy R2Sonic片段
    XTF_HEADER_R2SONIC_BATHY        = 67,  // Triton R2Sonic测深
    XTF_HEADER_R2SONIC_FTS          = 68,  // Triton R2Sonic片段
    XTF_HEADER_CODA_ECHOSCOPE_DATA  = 70,  // CODA Echoscope数据
    XTF_HEADER_CODA_ECHOSCOPE_CONFIG= 71,  // CODA配置
    XTF_HEADER_CODA_ECHOSCOPE_IMAGE = 72,  // CODA图像
    XTF_HEADER_EDGETECH_4600        = 73,  // Edgetech 4600
    XTF_HEADER_RESON_7018_WATERCOLUMN = 78, // Reson 7018水体数据
    XTF_HEADER_POSITION             = 100, // 原始定位（Reson专用）
    XTF_HEADER_BATHY_PROC           = 102, // 处理的测深
    XTF_HEADER_ATTITUDE_PROC        = 103, // 处理的姿态
    XTF_HEADER_SINGLEBEAM_PROC      = 104, // 处理的单波束
    XTF_HEADER_AUX_PROC             = 105, // 处理的辅助数据
    XTF_HEADER_KLEIN3000_DATA_PAGE  = 106, // Klein 3000数据页
    XTF_HEADER_POS_RAW_NAVIGATION   = 107, // 原始导航
    XTF_HEADER_KLEINV4_DATA_PAGE    = 108, // Klein V4数据页

    // ===== 用户自定义 =====
    XTF_HEADER_USERDEFINED          = 200  // 用户自定义数据起始值
};

struct XTFPINGHEADER {
    // ===== 基础头 ===== (字节0-13)
    uint16_t MagicNumber;      // 0xFACE (字节0-1)
    uint8_t  HeaderType;       // XTFHeaderType枚举 (字节2)
    uint8_t  SubChannelNumber; // 含义随HeaderType变化 (字节3)
    uint16_t NumChansToFollow; // 后续通道数 (字节4-5)
    uint16_t Reserved1[2];     // 保留 (字节6-9)
    uint32_t NumBytesThisRecord; // 总字节数 (字节10-13)

    // ===== 时间戳 ===== (字节14-23)
    uint16_t Year;            // 年 (字节14-15)
    uint8_t  Month;           // 月 (字节16)
    uint8_t  Day;             // 日 (字节17)
    uint8_t  Hour;            // 时 (字节18)
    uint8_t  Minute;          // 分 (字节19)
    uint8_t  Second;          // 秒 (字节20)
    uint8_t  HSeconds;        // 百分之一秒 (字节21)
    uint16_t JulianDay;       // 年积日 (字节22-23)

    // ===== 事件计数 ===== (字节24-31)
    uint32_t EventNumber;     // 事件号 (字节24-27)
    uint32_t PingNumber;      // Ping号 (字节28-31)

    // ===== 声学参数 ===== (字节32-43)
    float    SoundVelocity;   // 声速(m/s) (字节32-35)
    float    OceanTide;       // 潮位(m) (字节36-39)
    uint32_t Reserved2;       // 保留 (字节40-43)

    // ===== CTD原始数据 ===== (字节44-59)
    float    ConductivityFreq; // 电导率Hz (字节44-47)
    float    TemperatureFreq; // 温度Hz (字节48-51)
    float    PressureFreq;     // 压力Hz (字节52-55)
    float    PressureTemp;     // 压力温度°C (字节56-59)

    // ===== 计算值 ===== (字节60-75)
    float    Conductivity;    // 电导率S/m (字节60-63)
    float    WaterTemperature; // 水温°C (字节64-67)
    float    Pressure;        // 压力psi (字节68-71)
    float    ComputedSoundVelocity; // 计算声速m/s (字节72-75)

    // ===== 磁力仪 ===== (字节76-87)
    float    MagX;            // X轴磁力mGauss (字节76-79)
    float    MagY;            // Y轴磁力mGauss (字节80-83)
    float    MagZ;            // Z轴磁力mGauss (字节84-87)

    // ===== 辅助值 ===== (字节88-119)
    float    AuxVal1;         // 辅助值1 (字节88-91)
    float    AuxVal2;         // 辅助值2 (字节92-95)
    float    AuxVal3;         // 辅助值3 (字节96-99)
    float    AuxVal4;         // 辅助值4 (字节100-103)
    float    AuxVal5;         // 辅助值5 (字节104-107)
    float    AuxVal6;         // 辅助值6 (字节108-111)
    float    SpeedLog;        // 计程仪速度knots (字节112-115)
    float    Turbidity;       // 浊度 (字节116-119)

    // ===== 导航 ===== (字节120-151)
    float    ShipSpeed;       // 船速knots (字节120-123)
    float    ShipGyro;        // 船陀螺° (字节124-127)
    double   ShipYcoordinate; // 船纬度/北坐标 (字节128-135)
    double   ShipXcoordinate; // 船经度/东坐标 (字节136-143)
    uint16_t ShipAltitude;    // 船高程dm (字节144-145)
    uint16_t ShipDepth;       // 船吃水dm (字节146-147)
    uint8_t  FixTimeHour;     // 定位时 (字节148)
    uint8_t  FixTimeMinute;   // 定位分 (字节149)
    uint8_t  FixTimeSecond;   // 定位秒 (字节150)
    uint8_t  FixTimeHsecond;  // 定位百分秒 (字节151)

    // ===== 传感器导航 ===== (字节152-183)
    float    SensorSpeed;      // 传感器速度knots (字节152-155)
    float    KP;              // 管线公里数 (字节156-159)
    double   SensorYcoordinate; // 传感器纬度/北坐标 (字节160-167)
    double   SensorXcoordinate; // 传感器经度/东坐标 (字节168-175)
    uint16_t SonarStatus;     // 声纳状态 (字节176-177)
    uint16_t RangeToFish;     // 到拖鱼斜距dm (字节178-179)
    uint16_t BearingToFish;   // 到拖鱼方位°×100 (字节180-181)
    uint16_t CableOut;        // 缆长m (字节182-183)

    // ===== 拖鱼参数 ===== (字节184-239)
    float    Layback;         // 水平拖距m (字节184-187)
    float    CableTension;    // 缆张力kg (字节188-191)
    float    SensorDepth;     // 传感器深度m (字节192-195)
    float    SensorPrimaryAltitude; // 主高度m (字节196-199)
    float    SensorAuxAltitude; // 辅助高度m (字节200-203)
    float    SensorPitch;     // 俯仰° (字节204-207)
    float    SensorRoll;      // 横滚° (字节208-211)
    float    SensorHeading;   // 航向° (字节212-215)
    float    Heave;           // 升沉m (字节216-219)
    float    Yaw;             // 偏航° (字节220-223)
    uint32_t AttitudeTimeTag; // 姿态时间标签ms (字节224-227)
    float    DOT;             // 偏航距m (字节228-231)
    uint32_t NavFixMilliseconds; // 定位时间ms (字节232-235)
    uint8_t  ComputerClockHour; // 计算机时钟时 (字节236)
    uint8_t  ComputerClockMinute; // 计算机时钟分 (字节237)
    uint8_t  ComputerClockSecond; // 计算机时钟秒 (字节238)
    uint8_t  ComputerClockHsec; // 计算机时钟百分秒 (字节239)

    // ===== 扩展字段 ===== (字节240-255)
    int16_t  FishPositionDeltaX; // 拖鱼X偏移m×3 (字节240-241)
    int16_t  FishPositionDeltaY; // 拖鱼Y偏移m×3 (字节242-243)
    uint8_t  FishPositionErrorCode; // 定位错误码 (字节244)
    uint32_t OptionalOffset;   // 保留 (字节245-248)
    uint8_t  CableOutHundredths; // 缆长厘米数 (字节249)
    uint8_t  ReservedSpace2[6]; // 保留 (字节250-255)
};

// 通道编号标准枚举
enum XTFChannelNumber : uint16_t {
    CHAN_PORT_LOW_FREQ  = 0,   // 左舷低频通道（主通道）
    CHAN_STBD_LOW_FREQ  = 1,   // 右舷低频通道
    CHAN_PORT_HIGH_FREQ = 2,   // 左舷高频通道（双频系统）
    CHAN_STBD_HIGH_FREQ = 3,   // 右舷高频通道
};
// DownsampleMethod 枚举（根据Comment列）
enum DownsampleMethod : uint16_t {
    DOWNSAMPLE_MAX = 2,   // 最大值降采样
    DOWNSAMPLE_RMS = 4    // RMS降采样
};

// ProcessingFlags 枚举（根据Comment列）
enum ProcessingFlags : uint16_t {
    PROC_TVG = 0x0004,    // 时变增益
    PROC_BAC_GAC = 0x0008,// 底部/增益校正
    PROC_FILTER = 0x0010  // 滤波处理
};

struct XTFPINGCHANHEADER {
    // ===== 通道标识 =====
    uint16_t ChannelNumber;        // 通道号 (字节0-1)
    // 0=左舷低频, 1=右舷低频, 2=左舷高频...

    uint16_t DownsampleMethod;     // 降采样方法 (字节2-3)
    // 2=MAX, 4=RMS

    // ===== 距离参数 =====
    float    SlantRange;           // 斜距(m) (字节4-7)
    float    GroundRange;          // 地距(m) (字节8-11)
    // 计算公式：sqrt(SlantRange² - Altitude²)

    // ===== 时间参数 =====
    float    TimeDelay;            // 起始时间延迟(s) (字节12-15)
    // 通常为0.0
    float    TimeDuration;         // 记录时长(s) (字节16-19)
    // 典型值=SlantRange/750
    float    SecondsPerPing;       // Ping间隔(s) (字节20-23)
    // 通常等于TimeDuration

    // ===== 处理参数 =====
    uint16_t ProcessingFlags;      // 处理标志位 (字节24-25)
    // 位掩码：4=TVG, 8=BAC&GAC, 16=滤波
    uint16_t Frequency;            // 中心频率(Hz) (字节26-27)
    uint16_t InitialGainCode;      // 初始增益码 (字节28-29)
    // 来自声纳原始设置
    uint16_t GainCode;             // 当前增益码 (字节30-31)
    uint16_t BandWidth;            // 带宽(Hz) (字节32-33)

    // ===== 目标信息 =====
    uint32_t ContactNumber;        // 目标编号 (字节34-37)
    // TargetPro软件分配
    uint16_t ContactClassification; // 目标分类 (字节38-39)
    uint8_t  ContactSubNumber;     // 子目标编号 (字节40)
    uint8_t  ContactType;          // 目标类型 (字节41)

    // ===== 采样信息 =====
    uint32_t NumSamples;           // 样本数 (字节42-45)
    // =后续数据区的样本总数
    uint16_t MillivoltScale;       // 毫伏比例 (字节46-47)
    // 0表示使用VoltScale
    float    ContactTimeOffTrack;  // 目标偏离时间(ms) (字节48-51)
    uint8_t  ContactCloseNumber;   // 闭合目标编号 (字节52)
    uint8_t  Reserved2;            // 保留 (字节53)

    // ===== 扩展参数 =====
    float    FixedVSOP;            // 固定沿航迹尺寸(cm) (字节54-57)
    // 用于零波束扩展的多波束系统
    int16_t  Weight;               // 权重因子 (字节58-59)
    // Edgetech/Kongsberg专用
    uint8_t  ReservedSpace[4];     // 保留 (字节60-63)
};


// SubChannelNumber 枚举（根据Comment列）
enum HighSpeedSensorType : uint8_t {
    SENSOR_ALTITUDE = 0,   // 高度计数据
    SENSOR_ROLL    = 1,    // 横滚数据
    SENSOR_YAW     = 2     // 偏航数据
};

struct XTFHIGHSPEEDSENSOR {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须设置为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须设置为15 (XTFHIGHSPEEDSENSOR) (字节2)
    uint8_t  SubChannelNumber;  // 传感器类型 (HighSpeedSensorType枚举) (字节3)
    uint16_t NumChansToFollow; // 未使用（设置为0） (字节4-5)
    uint16_t Reserved1[2];     // 保留字段（设置为0） (字节6-9)
    uint32_t NumBytesThisRecord; // 数据包总字节数（含包头） (字节10-13)

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节14-15)
    uint8_t  Month;            // 月 (1-12) (字节16)
    uint8_t  Day;              // 日 (1-31) (字节17)
    uint8_t  Hour;             // 时 (0-23) (字节18)
    uint8_t  Minute;           // 分 (0-59) (字节19)
    uint8_t  Second;           // 秒 (0-59) (字节20)
    uint8_t  HSeconds;         // 百分之一秒 (0-99) (字节21)

    // ===== 传感器数据 =====
    uint32_t NumSensorBytes;   // 后续传感器数据字节数 (字节22-25)
    uint32_t RelativeBathyPingNum; // 关联的测深Ping号 (字节26-29)

    // ===== 保留区域 =====
    uint8_t  Reserved3[34];    // 保留字段（设置为0） (字节30-63)
};


struct XTFBEAMXYZA {
    // ===== 位置信息 =====
    double dPosOffsetTrX;   // 相对于换能器的东向偏移(m) (字节0-7)
    double dPosOffsetTrY;   // 相对于换能器的北向偏移(m) (字节8-15)
    float  fDepth;         // 绝对深度(m) (正值为水下) (字节16-19)

    // ===== 声学特性 =====
    double dTime;          // 双程传播时间(s) (字节20-27)
    int16_t usAmpl;        // 回波幅度(dB) (字节28-29)
    int8_t ucQuality; // 数据质量 (字节30)

    // ===== 扩展字段 =====
    uint8_t Reserved;      // 保留字段 (设置为0) (字节31)
};


struct SNP0 {
    // ===== 头标识 =====
    uint32_t ID;               // 固定值0x534E5030（'SNP0' ASCII） (字节0-3)
    uint16_t HeaderSize;       // 头大小（固定74字节） (字节4-5)
    uint16_t DataSize;        // 后续数据大小（字节） (字节6-7)

    // ===== 采集信息 =====
    uint32_t PingNumber;      // Ping序号 (字节8-11)
    uint32_t Seconds;        // Unix时间戳（秒） (字节12-15)
    uint32_t Millisec;       // 毫秒部分 (字节16-19)
    uint16_t Latency;        // Ping到输出的延迟（ms） (字节20-21)

    // ===== 设备标识 =====
    uint16_t SonarID[2];     // 声纳ID（以太网MAC后4字节） (字节22-25)
    uint16_t SonarModel;     // 声纳型号代码 (字节26-27)

    // ===== 声学参数 =====
    uint16_t Frequency;      // 频率（kHz） (字节28-29)
    uint16_t SSpeed;         // 设定的声速（m/s） (字节30-31)
    uint16_t SampleRate;     // 采样率（Hz） (字节32-33)
    uint16_t PingRate;       // Ping率（0.001Hz步进） (字节34-35)
    uint16_t Range;          // 量程（m） (字节36-37)
    uint16_t Power;          // 发射功率 (字节38-39)
    uint16_t Gain;           // 增益（bit15=自动, bit14=TVG） (字节40-41)
    uint16_t PulseWidth;     // 脉冲宽度（μs） (字节42-43)
    uint16_t Spread;     // TVG扩展系数 (字节44-45)
    uint16_t Absorb;         // TVG吸收系数（dB/km） (字节46-47)

    // ===== 波束配置 =====
    uint16_t Proj;    // 发射器配置 (字节48-49)
    uint16_t ProjWidth;      // 发射波束水平宽度（0.1°步进） (字节50-51)
    uint16_t SpacingNum;     // 接收波束间隔分子（°） (字节52-53)
    uint16_t SpacingDen;     // 接收波束间隔分母 (字节54-55)
    int16_t ProjAngle;       // 发射器转向角（°） (字节56-57)

    // ===== 过滤器设置 =====
    uint16_t MinRange;       // 最小量程过滤 (字节58-59)
    uint16_t MaxRange;       // 最大量程过滤 (字节60-61)
    uint16_t MinDepth;       // 最小深度过滤 (字节62-63)
    uint16_t MaxDepth;       // 最大深度过滤 (字节64-65)
    uint16_t Filters;        // 启用过滤器标志 (字节66-67)
    // bit0: 量程过滤, bit1: 深度过滤

    // ===== 状态标志 =====
    uint16_t bFlags;         // 系统标志 (字节68-69)
    // bit0: 横滚稳定启用, bit12-14: Snippet模式

    // ===== 环境参数 =====
    int16_t HeadTemp;        // 声纳头温度（0.1°C步进） (字节70-71)
    uint16_t BeamCnt;       // 波束数 (字节72-73)
};


// Snippet增益模式枚举（根据Comment列的GainStart/GainEnd字段）
enum SnippetGainMode : uint16_t {
    GAIN_FIXED = 0,      // 固定增益（忽略GainStart/End）
    GAIN_LINEAR = 1,     // 线性变化增益
    GAIN_CURVE = 2       // 自定义曲线增益
};

struct SNP1 {
    // ===== 头标识 =====
    uint32_t ID;               // 固定值0x534E5031（'SNP1' ASCII） (字节0-3)
    uint16_t HeaderSize;       // 头大小（固定24字节） (字节4-5)
    uint16_t DataSize;        // 后续数据大小（字节） (字节6-7)

    // ===== 波束信息 =====
    uint32_t PingNumber;      // 关联的Ping序号 (字节8-11)
    uint16_t Beam;           // 波束编号（0到N-1） (字节12-13)
    uint16_t SnipSamples;    // Snippet样本数 (字节14-15)

    // ===== 增益控制 =====
    uint16_t GainStart;      // 起始增益（0.01dB步进） (字节16-17)
    uint16_t GainEnd;        // 结束增益（0.01dB步进） (字节18-19)

    // ===== 数据定位 =====
    uint16_t FragOffset;     // 片段在Ping中的偏移（样本数） (字节20-21)
    uint16_t FragSamples;    // 片段样本数 (字节22-23)
};



// 时间标志枚举（根据Comment列的TimeFlag字段）
enum TimeFlag : uint8_t {
    TIME_RECEIVE_ONLY = 0,    // 仅接收时间有效
    TIME_SOURCE_ONLY  = 1,    // 仅源时间有效
    TIME_BOTH_VALID   = 3     // 两者均有效
};

struct XTFPOSRAWNAVIGATION {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为107 (XTF_HEADER_POS_RAW_NAVIGATION) (字节2)
    uint8_t  SubChannelNumber; // 保留（设置为0） (字节3)
    uint16_t NumChansToFollow; // 未使用（设置为0） (字节4-5)
    uint16_t Reserved1[2];     // 保留（设置为0） (字节6-9)
    uint32_t NumBytesThisRecord; // 必须为64（固定包大小） (字节10-13)

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节14-15)
    uint8_t  Month;            // 月 (字节16)
    uint8_t  Day;              // 日 (字节17)
    uint8_t  Hour;             // 时 (字节18)
    uint8_t  Minute;           // 分 (字节19)
    uint8_t  Second;           // 秒 (字节20)
    uint16_t MicroSeconds;     // 微秒（0-9999） (字节21-22)

    // ===== 原始导航数据 =====
    double   RawYcoordinate;   // 原始纬度/北坐标 (字节23-30)
    double   RawXcoordinate;   // 原始经度/东坐标 (字节31-38)
    double   RawAltitude;       // 原始高程（含RTK） (字节39-46)

    // ===== 姿态数据 =====
    float    Pitch;            // 俯仰角（°）正=船头上扬 (字节47-50)
    float    Roll;             // 横滚角（°）正=右舷下倾 (字节51-54)
    float    Heave;            // 升沉（m）正=传感器上升 (字节55-58)
    float    Heading;          // 航向（°）0=正北 (字节59-62)

    // ===== 状态标志 =====
    TimeFlag TimeFlag;         // 时间有效性标志 (字节63)
};





struct XTFQPSSINGLEBEAM {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为26 (XTF_HEADER_Q_SINGLEBEAM) (字节2)
    uint8_t  SubChannelNumber; // 换能器ID (字节3)
    uint16_t NumChansToFollow; // 未使用（设置为0） (字节4-5)
    uint16_t Reserved1[2];     // 保留（设置为0） (字节6-9)
    uint32_t NumBytesThisRecord; // 总字节数（固定64） (字节10-13)

    // ===== 时间与标识 =====
    uint32_t TimeTag;          // 系统时间戳（毫秒） (字节14-17)
    int      Id;               // 波束唯一ID (字节18-21)

    // ===== 声学参数 =====
    float    SoundVelocity;    // 声速（m/s） (字节22-25)
    float    Intensity;        // 回波强度 (dB) (字节26-29)
    int Quality;        // 数据质量 (字节30-33)

    // ===== 传播时间 =====
    float    TwoWayTravelTime; // 双程传播时间（秒） (字节34-37)

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节38-39)
    uint8_t  Month;            // 月 (字节40)
    uint8_t  Day;              // 日 (字节41)
    uint8_t  Hour;             // 时 (字节42)
    uint8_t  Minute;           // 分 (字节43)
    uint8_t  Second;           // 秒 (字节44)
    uint16_t MilliSeconds;     // 毫秒 (字节45-46)

    // ===== 保留字段 =====
    uint8_t  Reserved[7];      // 保留（设置为0） (字节47-53)
};




struct XTFQPSMULTITXENTRY {
    // ===== 基础信息 =====
    int      Id;               // 换能器ID (字节0-3)
    float    Intensity;        // 回波强度 (dB) (字节4-7)
    int      Quality;          // 数据质量 (字节8-11)
    // Comment: "Quality: 0=unchecked, 1=good, 2=suspect, 3=bad"

    // ===== 传播时间 =====
    float    TwoWayTravelTime; // 双程时间 (秒) (字节12-15)
    float    DeltaTime;        // 相对于头部的时差 (秒) (字节16-19)

    // ===== 换能器位置 =====
    float    OffsetX;          // X偏移 (右舷为正) (米) (字节20-23)
    float    OffsetY;          // Y偏移 (船头为正) (米) (字节24-27)
    float    OffsetZ;          // Z偏移 (向下为正) (米) (字节28-31)

    // ===== 保留字段 =====
    float    Reserved[4];      // 保留 (必须置零) (字节32-47)
};


struct XTFSEGYHEADER {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为108 (XTF_HEADER_SEGY) (字节2)
    uint8_t  SubChannelNumber; // 子通道号 (字节3)
    uint16_t NumChansToFollow; // 未使用（设置为0） (字节4-5)
    uint16_t Reserved1[2];     // 保留（设置为0） (字节6-9)
    uint32_t NumBytesThisRecord; // 总字节数（含SEGY数据） (字节10-13)

    // ===== SEGY参数 =====
    uint16_t TapeNumber;       // 磁带编号 (字节14-15)
    uint16_t FormatCode;       // 格式代码 (字节16-17)
    // Comment列明确值：
    // 1=FLOAT_IBM, 2=FLOAT_IEEE
    uint16_t SamplesPerTrace;  // 每道样本数 (字节18-19)
    uint16_t SampleInterval;   // 采样间隔(μs) (字节20-21)
    uint16_t SEGYRevision;     // SEGY版本号 (字节22-23)
    uint16_t FixedLengthTrace; // 固定长度标记 (字节24-25)
    // Comment: "0=可变长度,1=固定长度"
    uint16_t ExtendedHeaders; // 扩展头数量 (字节26-27)

    // ===== 保留区域 =====
    uint8_t  Reserved2[32];    // 保留（设置为0） (字节28-59)
    char     Description[32];  // 描述文本 (字节60-91)
    uint8_t  Reserved3[36];    // 保留（设置为0） (字节92-127)
};


struct XTFQPSMBEENTRY {
    // ===== 波束基础信息 =====
    int      Id;                // 波束ID (字节0-3)
    // Comment: "Beam ID"
    double   Intensity;         // 信号强度 (字节4-11)
    // Comment: "Signal Strength"
    int      Quality;           // 数据质量 (字节12-15)
    // Comment: "Quality"

    // ===== 时间参数 =====
    double   TwoWayTravelTime;  // 双程传播时间(秒) (字节16-23)
    // Comment: "Two way travel time in seconds"
    double   DeltaTime;         // 波束时间偏移 (字节24-31)
    // Comment: "Beam time offset"

    // ===== 角度参数 =====
    double   BeamAngle;         // 波束角 (字节32-39)
    // Comment: "Beam angle"
    double   TiltAngle;         // 倾斜角 (字节40-47)
    // Comment: "Tilt angle"

    // ===== 保留区域 =====
    float    Reserved[4];       // 保留字段 (字节48-63)
    // Comment: "Reserved"
};


/**
 * 制造商ID枚举 (XTF格式文档Rev.37 - 3.1.17节)
 * 注：所有厂商均为水下声学/测绘设备领域的专业制造商
 */
enum ManufacturerID : uint8_t {
    /**
     * [1] Benthos, Inc. (隶属Teledyne Marine集团)
     * 专注水下声学释放器、水听器等设备
     * 代表产品：C3D系列、SIS系列侧扫声纳
     */
    BENTHOS        = 1,

    /**
     * [2] Reson A/S (现属Kongsberg Maritime)
     * 多波束声纳技术领导者
     * 代表产品：Seabat系列、T系列多波束
     */
    RESON          = 2,

    /**
     * [3] EdgeTech (现属L3Harris)
     * 侧扫声纳与浅地层剖面仪专家
     * 代表产品：4200系列、4600系列
     */
    EDGETECH       = 3,

    /**
     * [4] Klein Associates (现属Kongsberg Maritime)
     * 侧扫声纳技术先驱
     * 代表产品：Klein 3000/3500/5000系列
     */
    KLEIN          = 4,

    /**
     * [5] CODA Octopus
     * 水下3D声学成像技术
     * 代表产品：Echoscope实时3D声纳
     */
    CODA           = 5,

    /**
     * [6] Kongsberg Maritime
     * 综合海洋技术供应商
     * 代表产品：EM系列多波束、HISAS合成孔径声纳
     */
    KONGSBERG      = 6,

    /**
     * [7] CMAX Survey
     * 澳大利亚水下测绘解决方案商
     * 专长多波束系统集成与导航软件
     */
    CMAX           = 7,

    /**
     * [8] Marine Sonic Technology (现属Teledyne Marine)
     * 高分辨率侧扫声纳制造商
     * 代表产品：SeaScan系列
     */
    MARINE_SONICS  = 8,

    /**
     * [9] Applied Signal Technology (现属Raytheon)
     * 军用级信号处理技术
     * 涉及水下通信与监测系统
     */
    APPLIED_SIGNAL = 9,

    /**
     * [10] Imagenex Technology
     * 经济型声纳设备供应商
     * 代表产品：DeltaT多波束、YellowFin剖面仪
     */
    IMAGENEX       = 10,

    /**
     * [11] GeoAcoustics Ltd (现属Teledyne Marine)
     * 海底地质测绘专家
     * 代表产品：GeoSwath系列宽幅多波束
     */
    GEOACOUSTICS   = 11
};
struct XTFRAWCUSTOMHEADER {
    // ===== 基础头 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为199 (字节2)
    uint8_t  ManufacturerID;  // 制造商ID (字节3)
    uint16_t SonarID;          // 声纳型号 (字节4-5)
    uint16_t PacketID;         // 数据包ID (字节6-7)
    uint16_t Reserved1;        // 保留 (字节8-9)
    uint32_t NumBytesThisRecord; // 总字节数 (字节10-13)

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节14-15)
    uint8_t  Month;            // 月 (字节16)
    uint8_t  Day;              // 日 (字节17)
    uint8_t  Hour;             // 时 (字节18)
    uint8_t  Minute;           // 分 (字节19)
    uint8_t  Second;           // 秒 (字节20)
    uint8_t  HSeconds;         // 百分之一秒 (字节21)
    uint16_t JulianDay;        // 年积日 (字节22-23)
    uint16_t Reserved2[2];     // 保留 (字节24-27)

    // ===== 数据标识 =====
    uint32_t PingNumber;       // Ping序号 (字节28-31)
    uint32_t TimeTag;          // 时间标签 (字节32-35)
    uint32_t NumCustomerBytes; // 自定义数据长度 (字节36-39)

    // ===== 保留区域 =====
    uint8_t  Reserved3[24];    // 填充至64字节 (字节40-63)
};


struct XTFHEADERNAVIGATION {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为42 (XTF_HEADER_NAVIGATION) (字节2)
    uint8_t  Reserved[7];      // 保留字段 (字节3-9)
    // Comment: "Must be here!"

    // ===== 数据包信息 =====
    uint32_t NumBytesThisRecord; // 总字节数（含头和数据） (字节10-13)
    // Comment: "Isis记录数据包为64字节倍数"

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节14-15)
    uint8_t  Month;            // 月 (字节16)
    uint8_t  Day;              // 日 (字节17)
    uint8_t  Hour;             // 时 (字节18)
    uint8_t  Minute;           // 分 (字节19)
    uint8_t  Second;           // 秒 (字节20)
    uint32_t Microseconds;     // 微秒 (0-999999) (字节21-24)

    // ===== 导航数据 =====
    uint32_t SourceEpoch;      // 从1970/1/1开始的秒数 (字节25-28)
    uint32_t TimeTag;          // 系统时间标签（毫秒） (字节29-32)
    double   RawYCoordinate;   // 原始Y坐标（纬度/北向） (字节33-40)
    double   RawXCoordinate;   // 原始X坐标（经度/东向） (字节41-48)
    double   RawAltitude;      // 原始高度 (字节49-56)

    // ===== 状态标志 =====
    uint8_t  TimeFlag;         // 时间戳有效性标志 (字节57)
    // Comment列明确值：
    // 0=仅接收时间有效
    // 1=仅源时间有效
    // 3=两者都有效

    uint8_t  Reserved1[6];     // 保留字段 (字节58-63)
    // Comment: "填充使结构体为64字节"
};


struct XTFHEADERGYRO {
    // ===== 基础头信息 =====
    uint16_t MagicNumber;      // 必须为0xFACE (字节0-1)
    uint8_t  HeaderType;       // 必须为84 (XTF_HEADER_SOURCETIME_GYRO) (字节2)
    uint8_t  Reserved[7];      // 保留字段 (字节3-9)
    // Comment: "Must be here!"

    // ===== 数据包信息 =====
    uint32_t NumBytesThisRecord; // 总字节数（含头和数据） (字节10-13)
    // Comment: "Isis记录数据包为64字节倍数"

    // ===== 时间戳 =====
    uint16_t Year;             // 年 (字节14-15)
    uint8_t  Month;            // 月 (字节16)
    uint8_t  Day;              // 日 (字节17)
    uint8_t  Hour;             // 时 (字节18)
    uint8_t  Minute;           // 分 (字节19)
    uint8_t  Second;           // 秒 (字节20)
    uint32_t Microseconds;     // 微秒 (0-999999) (字节21-24)

    // ===== 源时间信息 =====
    uint32_t SourceEpoch;      // 从1970/1/1开始的秒数 (字节25-28)
    uint32_t TimeTag;          // 系统时间标签（毫秒） (字节29-32)

    // ===== 姿态数据 =====
    float    Gyro;            // 原始航向（0-360度） (字节33-36)
    uint8_t  TimeFlag;        // 时间戳有效性标志 (字节37)
    // Comment列明确值：
    // 0=仅接收时间有效
    // 1=仅源时间有效
    // 3=两者都有效

    uint8_t  Reserved1[26];    // 保留字段 (字节38-63)
    // Comment: "填充使结构体为64字节"
};

#pragma pack(pop)


#endif // XTF_H
