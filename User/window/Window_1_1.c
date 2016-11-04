/*
*********************************************************************************************************
*
*	模块名称 : 程序编辑窗口模块
*	文件名称 : Window_1_1.c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2016-06-30 方川  正式发布
*
*	Copyright (C), 2015-2020, 阿波罗科技 www.apollorobot.cn
*
*********************************************************************************************************
*/

#include "Window_1_1.h"
#include "_apollorobot.h"
#include "os.h"
#include "common.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_HEADER_0     (GUI_ID_USER + 0x01)
#define ID_EDIT_0     	(GUI_ID_USER + 0x0A)
#define ID_EDIT_1     	(GUI_ID_USER + 0x0B)
#define ID_EDIT_2    		(GUI_ID_USER + 0x0C)
#define ID_EDIT_3     	(GUI_ID_USER + 0x0D)
#define ID_EDIT_4     	(GUI_ID_USER + 0x0E)
#define ID_EDIT_5     	(GUI_ID_USER + 0x0F)
#define ID_EDIT_6     	(GUI_ID_USER + 0x10)
#define ID_EDIT_7     	(GUI_ID_USER + 0x11)
#define ID_EDIT_8     	(GUI_ID_USER + 0x12)
#define ID_EDIT_9     	(GUI_ID_USER + 0x13)
#define ID_EDIT_10     	(GUI_ID_USER + 0x14)
#define ID_EDIT_11     	(GUI_ID_USER + 0x15)
#define ID_EDIT_12     	(GUI_ID_USER + 0x16)
#define ID_EDIT_13     	(GUI_ID_USER + 0x17)
#define ID_EDIT_14     	(GUI_ID_USER + 0x18)
#define ID_EDIT_15     	(GUI_ID_USER + 0x19)
#define ID_EDIT_16     	(GUI_ID_USER + 0x1A)
#define ID_EDIT_17     	(GUI_ID_USER + 0x1B)
#define ID_EDIT_18     	(GUI_ID_USER + 0x1C)
#define ID_EDIT_19     	(GUI_ID_USER + 0x1D)
#define ID_EDIT_20     	(GUI_ID_USER + 0x1E)
#define ID_EDIT_21     	(GUI_ID_USER + 0x1F)
#define ID_EDIT_22     	(GUI_ID_USER + 0x20)
#define ID_EDIT_23     	(GUI_ID_USER + 0x21)
#define ID_EDIT_24     	(GUI_ID_USER + 0x22)
#define ID_EDIT_25     	(GUI_ID_USER + 0x23)
#define ID_EDIT_26     	(GUI_ID_USER + 0x24)
#define ID_EDIT_27     	(GUI_ID_USER + 0x25)
#define ID_EDIT_28     	(GUI_ID_USER + 0x26)
#define ID_EDIT_29     	(GUI_ID_USER + 0x27)
#define ID_EDIT_30     	(GUI_ID_USER + 0x28)
#define ID_EDIT_31     	(GUI_ID_USER + 0x29)
#define ID_EDIT_32     	(GUI_ID_USER + 0x2A)
#define ID_EDIT_33     	(GUI_ID_USER + 0x2B)
#define ID_EDIT_34     	(GUI_ID_USER + 0x2C)
#define ID_EDIT_35     	(GUI_ID_USER + 0x2D)
#define ID_EDIT_36     	(GUI_ID_USER + 0x2E)
#define ID_EDIT_37     	(GUI_ID_USER + 0x2F)
#define ID_EDIT_38     	(GUI_ID_USER + 0x30)
#define ID_EDIT_39     	(GUI_ID_USER + 0x31)
#define ID_EDIT_40     	(GUI_ID_USER + 0x32)
#define ID_EDIT_41     	(GUI_ID_USER + 0x33)
#define ID_EDIT_42     	(GUI_ID_USER + 0x34)
#define ID_EDIT_43     	(GUI_ID_USER + 0x35)
#define ID_EDIT_44     	(GUI_ID_USER + 0x36)
#define ID_EDIT_45     	(GUI_ID_USER + 0x37)
#define ID_EDIT_46     	(GUI_ID_USER + 0x38)
#define ID_EDIT_47     	(GUI_ID_USER + 0x39)
#define ID_EDIT_48     	(GUI_ID_USER + 0x3A)
#define ID_EDIT_49     	(GUI_ID_USER + 0x3B)
#define ID_EDIT_50     	(GUI_ID_USER + 0x3C)
#define ID_EDIT_51     	(GUI_ID_USER + 0x3D)
#define ID_EDIT_52     	(GUI_ID_USER + 0x3E)
#define ID_EDIT_53     	(GUI_ID_USER + 0x3F)
#define ID_EDIT_54     	(GUI_ID_USER + 0x40)
#define ID_EDIT_55     	(GUI_ID_USER + 0x41)
#define ID_EDIT_56     	(GUI_ID_USER + 0x42)
#define ID_EDIT_57     	(GUI_ID_USER + 0x43)
#define ID_EDIT_58     	(GUI_ID_USER + 0x44)
#define ID_EDIT_59     	(GUI_ID_USER + 0x45)
#define ID_EDIT_60     	(GUI_ID_USER + 0x46)
#define ID_EDIT_61     	(GUI_ID_USER + 0x47)
#define ID_EDIT_62     	(GUI_ID_USER + 0x48)
#define ID_EDIT_63     	(GUI_ID_USER + 0x49)
#define ID_EDIT_64     	(GUI_ID_USER + 0x4A)
#define ID_EDIT_65     	(GUI_ID_USER + 0x4B)
#define ID_EDIT_66     	(GUI_ID_USER + 0x4C)
#define ID_EDIT_67     	(GUI_ID_USER + 0x4D)
#define ID_EDIT_68     	(GUI_ID_USER + 0x4E)
#define ID_EDIT_69     	(GUI_ID_USER + 0x4F)
#define ID_EDIT_70     	(GUI_ID_USER + 0x50)
#define ID_EDIT_71     	(GUI_ID_USER + 0x51)
#define ID_EDIT_72     	(GUI_ID_USER + 0x52)
#define ID_EDIT_73     	(GUI_ID_USER + 0x53)
#define ID_EDIT_74     	(GUI_ID_USER + 0x54)
#define ID_EDIT_75     	(GUI_ID_USER + 0x55)
#define ID_EDIT_76     	(GUI_ID_USER + 0x56)
#define ID_EDIT_77     	(GUI_ID_USER + 0x57)
#define ID_EDIT_78     	(GUI_ID_USER + 0x58)
#define ID_EDIT_79     	(GUI_ID_USER + 0x59)
#define ID_EDIT_80     	(GUI_ID_USER + 0x5A)
#define ID_EDIT_81     	(GUI_ID_USER + 0x5B)
#define ID_EDIT_82     	(GUI_ID_USER + 0x5C)
#define ID_EDIT_83     	(GUI_ID_USER + 0x5D)
#define ID_EDIT_84     	(GUI_ID_USER + 0x5E)
#define ID_EDIT_85     	(GUI_ID_USER + 0x5F)
#define ID_EDIT_86     	(GUI_ID_USER + 0x60)
#define ID_EDIT_87     	(GUI_ID_USER + 0x61)
#define ID_EDIT_88     	(GUI_ID_USER + 0x62)
#define ID_EDIT_89     	(GUI_ID_USER + 0x63)
#define ID_EDIT_90     	(GUI_ID_USER + 0x64)
#define ID_EDIT_91     	(GUI_ID_USER + 0x65)
#define ID_EDIT_92     	(GUI_ID_USER + 0x66)
#define ID_EDIT_93     	(GUI_ID_USER + 0x67)
#define ID_EDIT_94     	(GUI_ID_USER + 0x68)
#define ID_EDIT_95     	(GUI_ID_USER + 0x69)
#define ID_EDIT_96     	(GUI_ID_USER + 0x6A)
#define ID_EDIT_97     	(GUI_ID_USER + 0x6B)
#define ID_EDIT_98     	(GUI_ID_USER + 0x6C)
#define ID_EDIT_99     	(GUI_ID_USER + 0x6D)
#define ID_EDIT_100     	(GUI_ID_USER + 0x6E)
#define ID_EDIT_101     	(GUI_ID_USER + 0x6F)
#define ID_EDIT_102    		(GUI_ID_USER + 0x70)
#define ID_EDIT_103     	(GUI_ID_USER + 0x71)
#define ID_EDIT_104     	(GUI_ID_USER + 0x72)
#define ID_EDIT_105     	(GUI_ID_USER + 0x73)
#define ID_EDIT_106     	(GUI_ID_USER + 0x74)
#define ID_EDIT_107     	(GUI_ID_USER + 0x75)
#define ID_EDIT_108     	(GUI_ID_USER + 0x76)
#define ID_EDIT_109     	(GUI_ID_USER + 0x77)
#define ID_EDIT_110     	(GUI_ID_USER + 0x78)
#define ID_EDIT_111     	(GUI_ID_USER + 0x79)
#define ID_EDIT_112     	(GUI_ID_USER + 0x7A)
#define ID_EDIT_113     	(GUI_ID_USER + 0x7B)
#define ID_EDIT_114     	(GUI_ID_USER + 0x7C)
#define ID_EDIT_115     	(GUI_ID_USER + 0x7D)
#define ID_EDIT_116     	(GUI_ID_USER + 0x7E)
#define ID_EDIT_117     	(GUI_ID_USER + 0x7F)
#define ID_EDIT_118     	(GUI_ID_USER + 0x80)
#define ID_EDIT_119     	(GUI_ID_USER + 0x81)
#define ID_EDIT_120     	(GUI_ID_USER + 0x82)
#define ID_EDIT_121     	(GUI_ID_USER + 0x83)
#define ID_EDIT_122     	(GUI_ID_USER + 0x84)
#define ID_EDIT_123     	(GUI_ID_USER + 0x85)
#define ID_EDIT_124     	(GUI_ID_USER + 0x86)
#define ID_EDIT_125     	(GUI_ID_USER + 0x87)
#define ID_EDIT_126     	(GUI_ID_USER + 0x88)
#define ID_EDIT_127     	(GUI_ID_USER + 0x89)
#define ID_EDIT_128     	(GUI_ID_USER + 0x8A)
#define ID_EDIT_129     	(GUI_ID_USER + 0x8B)
#define ID_EDIT_130     	(GUI_ID_USER + 0x8C)
#define ID_EDIT_131     	(GUI_ID_USER + 0x8D)
#define ID_EDIT_132     	(GUI_ID_USER + 0x8E)
#define ID_EDIT_133     	(GUI_ID_USER + 0x8F)
#define ID_EDIT_134     	(GUI_ID_USER + 0x90)
#define ID_EDIT_135     	(GUI_ID_USER + 0x91)
#define ID_EDIT_136     	(GUI_ID_USER + 0x92)
#define ID_EDIT_137     	(GUI_ID_USER + 0x93)
#define ID_EDIT_138     	(GUI_ID_USER + 0x94)
#define ID_EDIT_139     	(GUI_ID_USER + 0x95)
#define ID_EDIT_140     	(GUI_ID_USER + 0x96)
#define ID_EDIT_141     	(GUI_ID_USER + 0x97)
#define ID_EDIT_142     	(GUI_ID_USER + 0x98)
#define ID_EDIT_143     	(GUI_ID_USER + 0x99)
#define ID_EDIT_144     	(GUI_ID_USER + 0x9A)
#define ID_EDIT_145     	(GUI_ID_USER + 0x9B)
#define ID_EDIT_146     	(GUI_ID_USER + 0x9C)
#define ID_EDIT_147     	(GUI_ID_USER + 0x9D)
#define ID_EDIT_148     	(GUI_ID_USER + 0x9E)
#define ID_EDIT_149     	(GUI_ID_USER + 0x9F)
#define ID_EDIT_150     	(GUI_ID_USER + 0xA0)
#define ID_EDIT_151     	(GUI_ID_USER + 0xA1)
#define ID_EDIT_152     	(GUI_ID_USER + 0xA2)
#define ID_EDIT_153     	(GUI_ID_USER + 0xA3)
#define ID_EDIT_154     	(GUI_ID_USER + 0xA4)
#define ID_EDIT_155     	(GUI_ID_USER + 0xA5)
#define ID_EDIT_156     	(GUI_ID_USER + 0xA6)
#define ID_EDIT_157     	(GUI_ID_USER + 0xA7)
#define ID_EDIT_158     	(GUI_ID_USER + 0xA8)
#define ID_EDIT_159     	(GUI_ID_USER + 0xA9)
#define ID_EDIT_160     	(GUI_ID_USER + 0xAA)
#define ID_EDIT_161     	(GUI_ID_USER + 0xAB)
#define ID_EDIT_162     	(GUI_ID_USER + 0xAC)
#define ID_EDIT_163     	(GUI_ID_USER + 0xAD)
#define ID_EDIT_164     	(GUI_ID_USER + 0xAE)
#define ID_EDIT_165     	(GUI_ID_USER + 0xAF)
#define ID_EDIT_166     	(GUI_ID_USER + 0xB0)
#define ID_EDIT_167     	(GUI_ID_USER + 0xB1)
#define ID_EDIT_168     	(GUI_ID_USER + 0xB2)
#define ID_EDIT_169     	(GUI_ID_USER + 0xB3)
#define ID_EDIT_170     	(GUI_ID_USER + 0xB4)
#define ID_EDIT_171     	(GUI_ID_USER + 0xB5)
#define ID_EDIT_172     	(GUI_ID_USER + 0xB6)
#define ID_EDIT_173     	(GUI_ID_USER + 0xB7)
#define ID_EDIT_174     	(GUI_ID_USER + 0xB8)
#define ID_EDIT_175     	(GUI_ID_USER + 0xB9)
#define ID_EDIT_176     	(GUI_ID_USER + 0xBA)
#define ID_EDIT_177     	(GUI_ID_USER + 0xBB)
#define ID_EDIT_178     	(GUI_ID_USER + 0xBC)
#define ID_EDIT_179     	(GUI_ID_USER + 0xBD)
#define ID_EDIT_180     	(GUI_ID_USER + 0xBE)
#define ID_EDIT_181     	(GUI_ID_USER + 0xBF)
#define ID_EDIT_182     	(GUI_ID_USER + 0xC0)
#define ID_EDIT_183     	(GUI_ID_USER + 0xC1)
#define ID_EDIT_184     	(GUI_ID_USER + 0xC2)
#define ID_EDIT_185     	(GUI_ID_USER + 0xC3)
#define ID_EDIT_186     	(GUI_ID_USER + 0xC4)
#define ID_EDIT_187     	(GUI_ID_USER + 0xC5)
#define ID_EDIT_188     	(GUI_ID_USER + 0xC6)
#define ID_EDIT_189     	(GUI_ID_USER + 0xC7)
#define ID_EDIT_190     	(GUI_ID_USER + 0xC8)
#define ID_EDIT_191     	(GUI_ID_USER + 0xC9)
#define ID_EDIT_192     	(GUI_ID_USER + 0xCA)
#define ID_EDIT_193     	(GUI_ID_USER + 0xCB)
#define ID_EDIT_194     	(GUI_ID_USER + 0xCC)
#define ID_EDIT_195     	(GUI_ID_USER + 0xCD)
#define ID_EDIT_196     	(GUI_ID_USER + 0xCE)
#define ID_EDIT_197     	(GUI_ID_USER + 0xCF)

#define ID_TEXT_0     (GUI_ID_USER + 0xD0)
#define ID_TEXT_1     (GUI_ID_USER + 0xD1)
#define ID_TEXT_2     (GUI_ID_USER + 0xD2)
#define ID_TEXT_3     (GUI_ID_USER + 0xD3)
#define ID_TEXT_4     (GUI_ID_USER + 0xD4)
#define ID_TEXT_5     (GUI_ID_USER + 0xD5)
#define ID_TEXT_6     (GUI_ID_USER + 0xD6)
#define ID_TEXT_7     (GUI_ID_USER + 0xD7)
#define ID_TEXT_8     (GUI_ID_USER + 0xD8)
#define ID_TEXT_9     (GUI_ID_USER + 0xD9)
#define ID_TEXT_10     (GUI_ID_USER + 0xDA)
#define ID_TEXT_11     (GUI_ID_USER + 0xDB)
#define ID_TEXT_12     (GUI_ID_USER + 0xDC)
#define ID_TEXT_13     (GUI_ID_USER + 0xDD)
#define ID_TEXT_14     (GUI_ID_USER + 0xDE)
#define ID_TEXT_15     (GUI_ID_USER + 0xDF)
#define ID_TEXT_16     (GUI_ID_USER + 0xE0)
#define ID_TEXT_17     (GUI_ID_USER + 0xE1)
#define ID_TEXT_18     (GUI_ID_USER + 0xE2)
#define ID_TEXT_19     (GUI_ID_USER + 0xE3)
#define ID_TEXT_20     (GUI_ID_USER + 0xE4)
#define ID_TEXT_21     (GUI_ID_USER + 0xE5)
#define ID_TEXT_22     (GUI_ID_USER + 0xE6)
#define ID_TEXT_23     (GUI_ID_USER + 0xE7)
#define ID_TEXT_24     (GUI_ID_USER + 0xE8)
#define ID_TEXT_25     (GUI_ID_USER + 0xE9)
#define ID_TEXT_26     (GUI_ID_USER + 0xEA)
#define ID_TEXT_27     (GUI_ID_USER + 0xEB)
#define ID_TEXT_28     (GUI_ID_USER + 0xEC)
#define ID_TEXT_29     (GUI_ID_USER + 0xED)
#define ID_TEXT_30     (GUI_ID_USER + 0xEE)
#define ID_TEXT_31     (GUI_ID_USER + 0xEF)
#define ID_TEXT_32     (GUI_ID_USER + 0xF0)
#define ID_TEXT_33     (GUI_ID_USER + 0xF1)
#define ID_TEXT_34     (GUI_ID_USER + 0xF2)
#define ID_TEXT_35     (GUI_ID_USER + 0xF3)
#define ID_TEXT_36     (GUI_ID_USER + 0xF4)
#define ID_TEXT_37     (GUI_ID_USER + 0xF5)
#define ID_TEXT_38     (GUI_ID_USER + 0xF6)
#define ID_TEXT_39     (GUI_ID_USER + 0xF7)
#define ID_TEXT_40     (GUI_ID_USER + 0xF8)
#define ID_TEXT_41     (GUI_ID_USER + 0xF9)
#define ID_TEXT_42     (GUI_ID_USER + 0xFA)
#define ID_TEXT_43     (GUI_ID_USER + 0xFB)
#define ID_TEXT_44     (GUI_ID_USER + 0xFC)
#define ID_TEXT_45     (GUI_ID_USER + 0xFD)
#define ID_TEXT_46     (GUI_ID_USER + 0xFE)
#define ID_TEXT_47     (GUI_ID_USER + 0xFF)
#define ID_TEXT_48     (GUI_ID_USER + 0x100)
#define ID_TEXT_49     (GUI_ID_USER + 0x101)
#define ID_TEXT_50     (GUI_ID_USER + 0x102)
#define ID_TEXT_51     (GUI_ID_USER + 0x103)
#define ID_TEXT_52     (GUI_ID_USER + 0x104)
#define ID_TEXT_53     (GUI_ID_USER + 0x105)
#define ID_TEXT_54     (GUI_ID_USER + 0x106)
#define ID_TEXT_55     (GUI_ID_USER + 0x107)
#define ID_TEXT_56     (GUI_ID_USER + 0x108)
#define ID_TEXT_57     (GUI_ID_USER + 0x109)
#define ID_TEXT_58     (GUI_ID_USER + 0x10A)
#define ID_TEXT_59     (GUI_ID_USER + 0x10B)
#define ID_TEXT_60     (GUI_ID_USER + 0x10C)
#define ID_TEXT_61     (GUI_ID_USER + 0x10D)
#define ID_TEXT_62     (GUI_ID_USER + 0x10E)
#define ID_TEXT_63     (GUI_ID_USER + 0x10F)
#define ID_TEXT_64     (GUI_ID_USER + 0x110)
#define ID_TEXT_65     (GUI_ID_USER + 0x111)
#define ID_TEXT_66     (GUI_ID_USER + 0x112)
#define ID_TEXT_67     (GUI_ID_USER + 0x113)
#define ID_TEXT_68     (GUI_ID_USER + 0x114)
#define ID_TEXT_69     (GUI_ID_USER + 0x115)
#define ID_TEXT_70     (GUI_ID_USER + 0x116)
#define ID_TEXT_71     (GUI_ID_USER + 0x117)
#define ID_TEXT_72     (GUI_ID_USER + 0x118)
#define ID_TEXT_73     (GUI_ID_USER + 0x119)
#define ID_TEXT_74     (GUI_ID_USER + 0x11A)
#define ID_TEXT_75     (GUI_ID_USER + 0x11B)
#define ID_TEXT_76     (GUI_ID_USER + 0x11C)
#define ID_TEXT_77     (GUI_ID_USER + 0x11D)
#define ID_TEXT_78     (GUI_ID_USER + 0x11E)
#define ID_TEXT_79     (GUI_ID_USER + 0x11F)
#define ID_TEXT_80     (GUI_ID_USER + 0x120)
#define ID_TEXT_81     (GUI_ID_USER + 0x121)
#define ID_TEXT_82     (GUI_ID_USER + 0x122)
#define ID_TEXT_83     (GUI_ID_USER + 0x123)
#define ID_TEXT_84     (GUI_ID_USER + 0x124)
#define ID_TEXT_85     (GUI_ID_USER + 0x125)
#define ID_TEXT_86     (GUI_ID_USER + 0x126)
#define ID_TEXT_87     (GUI_ID_USER + 0x127)
#define ID_TEXT_88     (GUI_ID_USER + 0x128)
#define ID_TEXT_89     (GUI_ID_USER + 0x129)
#define ID_TEXT_90     (GUI_ID_USER + 0x12A)
#define ID_TEXT_91     (GUI_ID_USER + 0x12B)
#define ID_TEXT_92     (GUI_ID_USER + 0x12C)
#define ID_TEXT_93     (GUI_ID_USER + 0x12D)
#define ID_TEXT_94     (GUI_ID_USER + 0x12E)
#define ID_TEXT_95     (GUI_ID_USER + 0x12F)
#define ID_TEXT_96     (GUI_ID_USER + 0x130)
#define ID_TEXT_97     (GUI_ID_USER + 0x131)
#define ID_TEXT_98     (GUI_ID_USER + 0x132)
#define ID_TEXT_99     (GUI_ID_USER + 0x133)
#define ID_TEXT_100     (GUI_ID_USER + 0x134)
#define ID_TEXT_101     (GUI_ID_USER + 0x135)
#define ID_TEXT_102     (GUI_ID_USER + 0x136)
#define ID_TEXT_103     (GUI_ID_USER + 0x137)
#define ID_TEXT_104     (GUI_ID_USER + 0x138)
#define ID_TEXT_105     (GUI_ID_USER + 0x139)
#define ID_TEXT_106     (GUI_ID_USER + 0x13A)
#define ID_TEXT_107     (GUI_ID_USER + 0x13B)
#define ID_TEXT_108     (GUI_ID_USER + 0x13C)
#define ID_TEXT_109     (GUI_ID_USER + 0x13D)
#define ID_TEXT_110     (GUI_ID_USER + 0x13E)
#define ID_TEXT_111     (GUI_ID_USER + 0x13F)
#define ID_TEXT_112     (GUI_ID_USER + 0x140)
#define ID_TEXT_113     (GUI_ID_USER + 0x141)
#define ID_TEXT_114     (GUI_ID_USER + 0x142)
#define ID_TEXT_115     (GUI_ID_USER + 0x143)
#define ID_TEXT_116     (GUI_ID_USER + 0x144)
#define ID_TEXT_117     (GUI_ID_USER + 0x145)
#define ID_TEXT_118     (GUI_ID_USER + 0x146)
#define ID_TEXT_119     (GUI_ID_USER + 0x147)
#define ID_TEXT_120     (GUI_ID_USER + 0x148)
#define ID_TEXT_121     (GUI_ID_USER + 0x149)
#define ID_TEXT_122     (GUI_ID_USER + 0x14A)
#define ID_TEXT_123     (GUI_ID_USER + 0x14B)
#define ID_TEXT_124     (GUI_ID_USER + 0x14C)
#define ID_TEXT_125     (GUI_ID_USER + 0x14D)
#define ID_TEXT_126     (GUI_ID_USER + 0x14E)
#define ID_TEXT_127     (GUI_ID_USER + 0x14F)
#define ID_TEXT_128     (GUI_ID_USER + 0x150)
#define ID_TEXT_129     (GUI_ID_USER + 0x151)
#define ID_TEXT_130     (GUI_ID_USER + 0x152)
#define ID_TEXT_131     (GUI_ID_USER + 0x153)
#define ID_TEXT_132     (GUI_ID_USER + 0x154)
#define ID_TEXT_133     (GUI_ID_USER + 0x155)
#define ID_TEXT_134     (GUI_ID_USER + 0x156)
#define ID_TEXT_135     (GUI_ID_USER + 0x157)
#define ID_TEXT_136     (GUI_ID_USER + 0x158)
#define ID_TEXT_137     (GUI_ID_USER + 0x159)
#define ID_TEXT_138     (GUI_ID_USER + 0x15A)
#define ID_TEXT_139     (GUI_ID_USER + 0x15B)
#define ID_TEXT_140     (GUI_ID_USER + 0x15C)
#define ID_TEXT_141     (GUI_ID_USER + 0x15D)
#define ID_TEXT_142     (GUI_ID_USER + 0x15E)
#define ID_TEXT_143     (GUI_ID_USER + 0x15F)
#define ID_TEXT_144     (GUI_ID_USER + 0x160)
#define ID_TEXT_145     (GUI_ID_USER + 0x161)
#define ID_TEXT_146     (GUI_ID_USER + 0x162)
#define ID_TEXT_147     (GUI_ID_USER + 0x163)
#define ID_TEXT_148     (GUI_ID_USER + 0x164)
#define ID_TEXT_149     (GUI_ID_USER + 0x165)
#define ID_TEXT_150     (GUI_ID_USER + 0x166)
#define ID_TEXT_151     (GUI_ID_USER + 0x167)
#define ID_TEXT_152     (GUI_ID_USER + 0x168)
#define ID_TEXT_153     (GUI_ID_USER + 0x169)
#define ID_TEXT_154     (GUI_ID_USER + 0x16A)
#define ID_TEXT_155     (GUI_ID_USER + 0x16B)
#define ID_TEXT_156     (GUI_ID_USER + 0x16C)
#define ID_TEXT_157     (GUI_ID_USER + 0x16D)
#define ID_TEXT_158     (GUI_ID_USER + 0x16E)
#define ID_TEXT_159     (GUI_ID_USER + 0x16F)
#define ID_TEXT_160     (GUI_ID_USER + 0x170)
#define ID_TEXT_161     (GUI_ID_USER + 0x171)
#define ID_TEXT_162     (GUI_ID_USER + 0x172)
#define ID_TEXT_163     (GUI_ID_USER + 0x173)
#define ID_TEXT_164     (GUI_ID_USER + 0x174)
#define ID_TEXT_165     (GUI_ID_USER + 0x175)
#define ID_TEXT_166     (GUI_ID_USER + 0x176)
#define ID_TEXT_167     (GUI_ID_USER + 0x177)
#define ID_TEXT_168     (GUI_ID_USER + 0x178)
#define ID_TEXT_169     (GUI_ID_USER + 0x179)
#define ID_TEXT_170     (GUI_ID_USER + 0x17A)
#define ID_TEXT_171     (GUI_ID_USER + 0x17B)
#define ID_TEXT_172     (GUI_ID_USER + 0x17C)
#define ID_TEXT_173     (GUI_ID_USER + 0x17D)
#define ID_TEXT_174     (GUI_ID_USER + 0x17E)
#define ID_TEXT_175     (GUI_ID_USER + 0x17F)
#define ID_TEXT_176     (GUI_ID_USER + 0x180)
#define ID_TEXT_177     (GUI_ID_USER + 0x181)
#define ID_TEXT_178     (GUI_ID_USER + 0x182)
#define ID_TEXT_179     (GUI_ID_USER + 0x183)
#define ID_TEXT_180     (GUI_ID_USER + 0x184)
#define ID_TEXT_181     (GUI_ID_USER + 0x185)
#define ID_TEXT_182     (GUI_ID_USER + 0x186)
#define ID_TEXT_183     (GUI_ID_USER + 0x187)
#define ID_TEXT_184     (GUI_ID_USER + 0x188)
#define ID_TEXT_185     (GUI_ID_USER + 0x189)
#define ID_TEXT_186     (GUI_ID_USER + 0x18A)
#define ID_TEXT_187     (GUI_ID_USER + 0x18B)
#define ID_TEXT_188     (GUI_ID_USER + 0x18C)
#define ID_TEXT_189     (GUI_ID_USER + 0x18D)
#define ID_TEXT_190     (GUI_ID_USER + 0x18E)
#define ID_TEXT_191     (GUI_ID_USER + 0x18F)
#define ID_TEXT_192     (GUI_ID_USER + 0x190)
#define ID_TEXT_193     (GUI_ID_USER + 0x191)
#define ID_TEXT_194     (GUI_ID_USER + 0x192)
#define ID_TEXT_195     (GUI_ID_USER + 0x193)
#define ID_TEXT_196     (GUI_ID_USER + 0x194)
#define ID_TEXT_197     (GUI_ID_USER + 0x195)


// USER START (Optionally insert additional defines)
#define ID_BUTTON_BACK     (GUI_ID_USER + 0x03)
#define ID_BUTTON_RUN     (GUI_ID_USER + 0x04)
#define ID_SCROLLBAR_0  (GUI_ID_USER + 0X05)
#define MAX_EDIT_NUM      198
#define MAX_TEXT_NUM      198
// USER END
/*********************************************************************
*
*       Global data
*
**********************************************************************
*/

extern char _acText[50];
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSongTi12;
extern char program_name[10];		//the name of program
extern OS_SEM        RUN_SEM;		//定义一个信号量，用于点击“运行”按钮时同步运行任务


WM_HWIN hWin1_1;
EDIT_Handle hEdit[MAX_EDIT_NUM];
TEXT_Handle hText[MAX_TEXT_NUM];

volatile int Edit_Index;//文本框的索引,用于链表的插入
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static const char*StringHZ[] = {
	"\xe8\xbf\x90\xe8\xa1\x8c","\xe8\xbf\x94\xe5\x9b\x9e",//0:运行，1:返回
	"\xe5\xbc\x80\xe5\xa7\x8b",//2:开始 
	"\xe4\xbf\x9d\xe5\xad\x98\xe5\xb9\xb6\xe8\xbf\x90\xe8\xa1\x8c?",//3:保存并运行?
	"\xe9\x80\x9a\xe7\x9f\xa5",//4:通知
	"\xe7\xa8\x8b\xe5\xba\x8f\xe4\xb8\xba\xe7\xa9\xba!",//5:程序为空!
	"\xe9\x94\x99\xe8\xaf\xaf",//6:错误
};
/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 1600, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "HeaderTop", ID_HEADER_0, 0, 0, 240, 20, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_0, 50, 30, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_1, 50, 60, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_2, 50, 90, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_3, 50, 120, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_4, 50, 150, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_5, 50, 180, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_6, 50, 210, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_7, 50, 240, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_8, 50, 270, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_9, 50, 300, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_10, 50, 330, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_11, 50, 360, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_12, 50, 390, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_13, 50, 420, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_14, 50, 450, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_15, 50, 480, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_16, 50, 510, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_17, 50, 540, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_18, 50, 570, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_19, 50, 600, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_20, 50, 630, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_21, 50, 660, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_22, 50, 690, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_23, 50, 720, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_24, 50, 750, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_25, 50, 780, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_26, 50, 810, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_27, 50, 840, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_28, 50, 870, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_29, 50, 900, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_30, 50, 930, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_31, 50, 960, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_32, 50, 990, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_33, 50, 1020, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_34, 50, 1050, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_35, 50, 1080, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_36, 50, 1110, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_37, 50, 1140, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_38, 50, 1170, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_39, 50, 1200, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_40, 50, 1230, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_41, 50, 1260, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_42, 50, 1290, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_43, 50, 1320, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_44, 50, 1350, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_45, 50, 1380, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_46, 50, 1410, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_47, 50, 1440, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_48, 50, 1470, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_49, 50, 1500, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_50, 50, 1530, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_51, 50, 1560, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_52, 50, 1590, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_53, 50, 1620, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_54, 50, 1650, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_55, 50, 1680, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_56, 50, 1710, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_57, 50, 1740, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_58, 50, 1770, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_59, 50, 1800, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_60, 50, 1830, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_61, 50, 1860, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_62, 50, 1890, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_63, 50, 1920, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_64, 50, 1950, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_65, 50, 1980, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_66, 50, 2010, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_67, 50, 2040, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_68, 50, 2070, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_69, 50, 2100, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_70, 50, 2130, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_71, 50, 2160, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_72, 50, 2190, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_73, 50, 2220, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_74, 50, 2250, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_75, 50, 2280, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_76, 50, 2310, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_77, 50, 2340, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_78, 50, 2370, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_79, 50, 2400, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_80, 50, 2430, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_81, 50, 2460, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_82, 50, 2490, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_83, 50, 2520, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_84, 50, 2550, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_85, 50, 2580, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_86, 50, 2610, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_87, 50, 2640, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_88, 50, 2670, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_89, 50, 2700, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_90, 50, 2730, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_91, 50, 2760, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_92, 50, 2790, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_93, 50, 2820, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_94, 50, 2850, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_95, 50, 2880, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_96, 50, 2910, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_97, 50, 2940, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_98, 50, 2970, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_99, 50, 3000, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_100, 50, 3030, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_101, 50, 3060, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_102, 50, 3090, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_103, 50, 3120, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_104, 50, 3150, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_105, 50, 3180, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_106, 50, 3210, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_107, 50, 3240, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_108, 50, 3270, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_109, 50, 3300, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_110, 50, 3330, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_111, 50, 3360, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_112, 50, 3390, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_113, 50, 3420, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_114, 50, 3450, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_115, 50, 3480, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_116, 50, 3510, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_117, 50, 3540, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_118, 50, 3570, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_119, 50, 3600, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_120, 50, 3630, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_121, 50, 3660, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_122, 50, 3690, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_123, 50, 3720, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_124, 50, 3750, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_125, 50, 3780, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_126, 50, 3810, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_127, 50, 3840, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_128, 50, 3870, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_129, 50, 3900, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_130, 50, 3930, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_131, 50, 3960, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_132, 50, 3990, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_133, 50, 4020, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_134, 50, 4050, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_135, 50, 4080, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_136, 50, 4110, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_137, 50, 4140, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_138, 50, 4170, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_139, 50, 4200, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_140, 50, 4230, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_141, 50, 4260, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_142, 50, 4290, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_143, 50, 4320, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_144, 50, 4350, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_145, 50, 4380, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_146, 50, 4410, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_147, 50, 4440, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "", ID_EDIT_148, 50, 4470, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_149, 50, 4500, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_150, 50, 4530, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_151, 50, 4560, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_152, 50, 4590, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_153, 50, 4620, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_154, 50, 4650, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_155, 50, 4680, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_156, 50, 4710, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_157, 50, 4740, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_158, 50, 4770, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_159, 50, 4800, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_160, 50, 4830, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_161, 50, 4860, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_162, 50, 4890, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_163, 50, 4920, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_164, 50, 4950, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_165, 50, 4980, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_166, 50, 5010, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_167, 50, 5040, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_168, 50, 5070, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_169, 50, 5100, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_170, 50, 5130, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_171, 50, 5160, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_172, 50, 5190, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_173, 50, 5220, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_174, 50, 5250, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_175, 50, 5280, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_176, 50, 5310, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_177, 50, 5340, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_178, 50, 5370, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_179, 50, 5400, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_180, 50, 5430, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_181, 50, 5460, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_182, 50, 5490, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_183, 50, 5520, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_184, 50, 5550, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_185, 50, 5580, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_186, 50, 5610, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_187, 50, 5640, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_188, 50, 5670, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_189, 50, 5700, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_190, 50, 5730, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_191, 50, 5760, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_192, 50, 5790, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_193, 50, 5820, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_194, 50, 5850, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_195, 50, 5880, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_196, 50, 5910, 120, 20, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_197, 50, 5940, 120, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "0", ID_TEXT_0, 0, 30, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "1", ID_TEXT_1, 0, 60, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "2", ID_TEXT_2, 0, 90, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "3", ID_TEXT_3, 0, 120, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "4", ID_TEXT_4, 0, 150, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "5", ID_TEXT_5, 0, 180, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "6", ID_TEXT_6, 0, 210, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "7", ID_TEXT_7, 0, 240, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "8", ID_TEXT_8, 0, 270, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "9", ID_TEXT_9, 0, 300, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "10", ID_TEXT_10, 0, 330, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "11", ID_TEXT_11, 0, 360, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "12", ID_TEXT_12, 0, 390, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "13", ID_TEXT_13, 0, 420, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "14", ID_TEXT_14, 0, 450, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "15", ID_TEXT_15, 0, 480, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "16", ID_TEXT_16, 0, 510, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "17", ID_TEXT_17, 0, 540, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "18", ID_TEXT_18, 0, 570, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "19", ID_TEXT_19, 0, 600, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "20", ID_TEXT_20, 0, 630, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "21", ID_TEXT_21, 0, 660, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "22", ID_TEXT_22, 0, 690, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "23", ID_TEXT_23, 0, 720, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "24", ID_TEXT_24, 0, 750, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "25", ID_TEXT_25, 0, 780, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "26", ID_TEXT_26, 0, 810, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "27", ID_TEXT_27, 0, 840, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "28", ID_TEXT_28, 0, 870, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "29", ID_TEXT_29, 0, 900, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "30", ID_TEXT_30, 0, 930, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "31", ID_TEXT_31, 0, 960, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "32", ID_TEXT_32, 0, 990, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "33", ID_TEXT_33, 0, 1020, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "34", ID_TEXT_34, 0, 1050, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "35", ID_TEXT_35, 0, 1080, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "36", ID_TEXT_36, 0, 1110, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "37", ID_TEXT_37, 0, 1140, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "38", ID_TEXT_38, 0, 1170, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "39", ID_TEXT_39, 0, 1200, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "40", ID_TEXT_40, 0, 1230, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "41", ID_TEXT_41, 0, 1260, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "42", ID_TEXT_42, 0, 1290, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "43", ID_TEXT_43, 0, 1320, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "44", ID_TEXT_44, 0, 1350, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "45", ID_TEXT_45, 0, 1380, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "46", ID_TEXT_46, 0, 1410, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "47", ID_TEXT_47, 0, 1440, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "48", ID_TEXT_48, 0, 1470, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "49", ID_TEXT_49, 0, 1500, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "50", ID_TEXT_50, 0, 1530, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "51", ID_TEXT_51, 0, 1560, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "52", ID_TEXT_52, 0, 1590, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "53", ID_TEXT_53, 0, 1620, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "54", ID_TEXT_54, 0, 1650, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "55", ID_TEXT_55, 0, 1680, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "56", ID_TEXT_56, 0, 1710, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "57", ID_TEXT_57, 0, 1740, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "58", ID_TEXT_58, 0, 1770, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "59", ID_TEXT_59, 0, 1800, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "60", ID_TEXT_60, 0, 1830, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "61", ID_TEXT_61, 0, 1860, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "62", ID_TEXT_62, 0, 1890, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "63", ID_TEXT_63, 0, 1920, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "64", ID_TEXT_64, 0, 1950, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "65", ID_TEXT_65, 0, 1980, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "66", ID_TEXT_66, 0, 2010, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "67", ID_TEXT_67, 0, 2040, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "68", ID_TEXT_68, 0, 2070, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "69", ID_TEXT_69, 0, 2100, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "70", ID_TEXT_70, 0, 2130, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "71", ID_TEXT_71, 0, 2160, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "72", ID_TEXT_72, 0, 2190, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "73", ID_TEXT_73, 0, 2220, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "74", ID_TEXT_74, 0, 2250, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "75", ID_TEXT_75, 0, 2280, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "76", ID_TEXT_76, 0, 2310, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "77", ID_TEXT_77, 0, 2340, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "78", ID_TEXT_78, 0, 2370, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "79", ID_TEXT_79, 0, 2400, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "80", ID_TEXT_80, 0, 2430, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "81", ID_TEXT_81, 0, 2460, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "82", ID_TEXT_82, 0, 2490, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "83", ID_TEXT_83, 0, 2520, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "84", ID_TEXT_84, 0, 2550, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "85", ID_TEXT_85, 0, 2580, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "86", ID_TEXT_86, 0, 2610, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "87", ID_TEXT_87, 0, 2640, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "88", ID_TEXT_88, 0, 2670, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "89", ID_TEXT_89, 0, 2700, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "90", ID_TEXT_90, 0, 2730, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "91", ID_TEXT_91, 0, 2760, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "92", ID_TEXT_92, 0, 2790, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "93", ID_TEXT_93, 0, 2820, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "94", ID_TEXT_94, 0, 2850, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "95", ID_TEXT_95, 0, 2880, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "96", ID_TEXT_96, 0, 2910, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "97", ID_TEXT_97, 0, 2940, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "98", ID_TEXT_98, 0, 2970, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "99", ID_TEXT_99, 0, 3000, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "100", ID_TEXT_100, 0, 3030, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "101", ID_TEXT_101, 0, 3060, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "102", ID_TEXT_102, 0, 3090, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "103", ID_TEXT_103, 0, 3120, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "104", ID_TEXT_104, 0, 3150, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "105", ID_TEXT_105, 0, 3180, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "106", ID_TEXT_106, 0, 3210, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "107", ID_TEXT_107, 0, 3240, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "108", ID_TEXT_108, 0, 3270, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "109", ID_TEXT_109, 0, 3300, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "110", ID_TEXT_110, 0, 3330, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "111", ID_TEXT_111, 0, 3360, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "112", ID_TEXT_112, 0, 3390, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "113", ID_TEXT_113, 0, 3420, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "114", ID_TEXT_114, 0, 3450, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "115", ID_TEXT_115, 0, 3480, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "116", ID_TEXT_116, 0, 3510, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "117", ID_TEXT_117, 0, 3540, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "118", ID_TEXT_118, 0, 3570, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "119", ID_TEXT_119, 0, 3600, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "120", ID_TEXT_120, 0, 3630, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "121", ID_TEXT_121, 0, 3660, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "122", ID_TEXT_122, 0, 3690, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "123", ID_TEXT_123, 0, 3720, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "124", ID_TEXT_124, 0, 3750, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "125", ID_TEXT_125, 0, 3780, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "126", ID_TEXT_126, 0, 3810, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "127", ID_TEXT_127, 0, 3840, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "128", ID_TEXT_128, 0, 3870, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "129", ID_TEXT_129, 0, 3900, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "130", ID_TEXT_130, 0, 3930, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "131", ID_TEXT_131, 0, 3960, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "132", ID_TEXT_132, 0, 3990, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "133", ID_TEXT_133, 0, 4020, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "134", ID_TEXT_134, 0, 4050, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "135", ID_TEXT_135, 0, 4080, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "136", ID_TEXT_136, 0, 4110, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "137", ID_TEXT_137, 0, 4140, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "138", ID_TEXT_138, 0, 4170, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "139", ID_TEXT_139, 0, 4200, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "140", ID_TEXT_140, 0, 4230, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "141", ID_TEXT_141, 0, 4260, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "142", ID_TEXT_142, 0, 4290, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "143", ID_TEXT_143, 0, 4320, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "144", ID_TEXT_144, 0, 4350, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "145", ID_TEXT_145, 0, 4380, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "146", ID_TEXT_146, 0, 4410, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "147", ID_TEXT_147, 0, 4440, 40, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "148", ID_TEXT_148, 0, 4470, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "149", ID_TEXT_149, 0, 4500, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "150", ID_TEXT_150, 0, 4530, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "151", ID_TEXT_151, 0, 4560, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "152", ID_TEXT_152, 0, 4590, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "153", ID_TEXT_153, 0, 4620, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "154", ID_TEXT_154, 0, 4650, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "155", ID_TEXT_155, 0, 4680, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "156", ID_TEXT_156, 0, 4710, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "157", ID_TEXT_157, 0, 4740, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "158", ID_TEXT_158, 0, 4770, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "159", ID_TEXT_159, 0, 4800, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "160", ID_TEXT_160, 0, 4830, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "161", ID_TEXT_161, 0, 4860, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "162", ID_TEXT_162, 0, 4890, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "163", ID_TEXT_163, 0, 4920, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "164", ID_TEXT_164, 0, 4950, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "165", ID_TEXT_165, 0, 4980, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "166", ID_TEXT_166, 0, 5010, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "167", ID_TEXT_167, 0, 5040, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "168", ID_TEXT_168, 0, 5070, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "169", ID_TEXT_169, 0, 5100, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "170", ID_TEXT_170, 0, 5130, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "171", ID_TEXT_171, 0, 5160, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "172", ID_TEXT_172, 0, 5190, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "173", ID_TEXT_173, 0, 5220, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "174", ID_TEXT_174, 0, 5250, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "175", ID_TEXT_175, 0, 5280, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "176", ID_TEXT_176, 0, 5310, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "177", ID_TEXT_177, 0, 5340, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "178", ID_TEXT_178, 0, 5370, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "179", ID_TEXT_179, 0, 5400, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "180", ID_TEXT_180, 0, 5430, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "181", ID_TEXT_181, 0, 5460, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "182", ID_TEXT_182, 0, 5490, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "183", ID_TEXT_183, 0, 5520, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "184", ID_TEXT_184, 0, 5550, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "185", ID_TEXT_185, 0, 5580, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "186", ID_TEXT_186, 0, 5610, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "187", ID_TEXT_187, 0, 5640, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "188", ID_TEXT_188, 0, 5670, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "189", ID_TEXT_189, 0, 5700, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "190", ID_TEXT_190, 0, 5730, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "191", ID_TEXT_191, 0, 5760, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "192", ID_TEXT_192, 0, 5790, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "193", ID_TEXT_193, 0, 5820, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "194", ID_TEXT_194, 0, 5850, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "195", ID_TEXT_195, 0, 5880, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "196", ID_TEXT_196, 0, 5910, 40, 20, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "197", ID_TEXT_197, 0, 5940, 40, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BACK", ID_BUTTON_BACK, 0, 290, 60, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "RUN", ID_BUTTON_RUN, 180, 290, 60, 30, 0, 0x0, 0 },
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	WM_SCROLL_STATE ScrollState;
	WM_HWIN hDlg;
	OS_ERR  err;
	static int _yOld=0;
	int i;
	GUI_RECT  rect;
	u8      Mb_Val;		
	GUI_PID_STATE State;

  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
		
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
		//
		//Initialize the scrollbar
		//
		SCROLLBAR_CreateEx(220,20,20,270, hItem, WM_CF_SHOW, SCROLLBAR_CF_VERTICAL, ID_SCROLLBAR_0);
		hItem = WM_GetDialogItem(pMsg->hWin,ID_SCROLLBAR_0);
		SCROLLBAR_SetNumItems(hItem,198);//设置滚动条项目数为200
		SCROLLBAR_SetPageSize(hItem, 9); //设置每页的项目数为9
    //
		// Initialize the 'Header'
		//
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_0);
    HEADER_AddItem(hItem, 80, program_name, 14);
		//
		//Initialize the 'Button'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_RUN);//RUN
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem,StringHZ[0]);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_BACK);//BACK
		BUTTON_SetFont(hItem,&GUI_FontSongTi12);
    BUTTON_SetText(hItem,StringHZ[1]);
    //
    // Initialization of Edit 1、2、3...
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
		EDIT_SetDefaultFont(&GUI_FontSongTi12);
		EDIT_SetFont(hItem, &GUI_FontSongTi12);
		EDIT_SetText(hItem, StringHZ[2]);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		WM_DisableWindow(hItem);

		for(i =0;i< MAX_EDIT_NUM;i++)
		{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0 + i);
				hEdit[i] = hItem;
				hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0 + i);
				TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
				hText[i] = hItem;
		}

		//如果链表不为空，说明是打开原来保存的程序，则将链表内容赋给EDIT
		if(Ins_List_Head -> next)
		{
			int listlength = GetListLength(Ins_List_Head);
			_Listptr p = Ins_List_Head -> next;
			for(i = 1; i <= listlength; i++)
			{
					EDIT_SetFont(hEdit[i], &GUI_FontSongTi12);
					EDIT_SetText(hEdit[i], p->EditContent );
					EDIT_SetBkColor(hEdit[i],EDIT_CI_ENABLED,GUI_CYAN);
					p = p -> next;
			}
		}
    
		WM_HideWindow(hEdit[9]);
		WM_HideWindow(hText[9]);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    GUI_TOUCH_GetState(&State);
	
    switch(Id) {
    case ID_EDIT_0: // Notifications sent by 'Edit0'
										switch(NCode) {
											default: break;
										}
      break;
    case ID_EDIT_1: // Notifications sent by 'Edit1'
    case ID_EDIT_2: // Notifications sent by 'Edit2'
    case ID_EDIT_3: // Notifications sent by 'Edit3'
    case ID_EDIT_4: // Notifications sent by 'Edit4'
    case ID_EDIT_5: // Notifications sent by 'Edit5'
    case ID_EDIT_6: // Notifications sent by 'Edit6'
    case ID_EDIT_7: // Notifications sent by 'Edit7'
    case ID_EDIT_8: // Notifications sent by 'Edit8'
		case ID_EDIT_9: // Notifications sent by 'Edit9'
		case ID_EDIT_10: // Notifications sent by 'Edit10'
		case ID_EDIT_11: // Notifications sent by 'Edit11'
		case ID_EDIT_12: // Notifications sent by 'Edit12'
		case ID_EDIT_13: // Notifications sent by 'Edit13'
		case ID_EDIT_14: // Notifications sent by 'Edit14'
		case ID_EDIT_15: // Notifications sent by 'Edit15'
		case ID_EDIT_16: // Notifications sent by 'Edit16'
		case ID_EDIT_17: // Notifications sent by 'Edit17'
		case ID_EDIT_18: // Notifications sent by 'Edit18'
		case ID_EDIT_19: // Notifications sent by 'Edit19'
		case ID_EDIT_20: // Notifications sent by 'Edit20'
		case ID_EDIT_21: // Notifications sent by 'Edit21'
		case ID_EDIT_22: // Notifications sent by 'Edit22'
		case ID_EDIT_23: // Notifications sent by 'Edit23'
		case ID_EDIT_24: // Notifications sent by 'Edit24'
		case ID_EDIT_25: // Notifications sent by 'Edit25'
		case ID_EDIT_26: // Notifications sent by 'Edit26'
		case ID_EDIT_27: // Notifications sent by 'Edit27'
		case ID_EDIT_28: // Notifications sent by 'Edit28'
		case ID_EDIT_29: // Notifications sent by 'Edit29'
		case ID_EDIT_30: // Notifications sent by 'Edit30'
		case ID_EDIT_31: // Notifications sent by 'Edit31'
		case ID_EDIT_32: // Notifications sent by 'Edit32'
		case ID_EDIT_33: // Notifications sent by 'Edit33'
		case ID_EDIT_34: // Notifications sent by 'Edit34'
		case ID_EDIT_35: // Notifications sent by 'Edit35'
		case ID_EDIT_36: // Notifications sent by 'Edit36'
		case ID_EDIT_37: // Notifications sent by 'Edit37'
		case ID_EDIT_38: // Notifications sent by 'Edit38'
		case ID_EDIT_39: // Notifications sent by 'Edit39'
		case ID_EDIT_40: // Notifications sent by 'Edit40'
		case ID_EDIT_41: // Notifications sent by 'Edit41'
		case ID_EDIT_42: // Notifications sent by 'Edit42'
		case ID_EDIT_43: // Notifications sent by 'Edit43'
		case ID_EDIT_44: // Notifications sent by 'Edit44'
		case ID_EDIT_45: // Notifications sent by 'Edit45'
		case ID_EDIT_46: // Notifications sent by 'Edit46'
		case ID_EDIT_47: // Notifications sent by 'Edit47'
		case ID_EDIT_48: // Notifications sent by 'Edit48'
		case ID_EDIT_49: // Notifications sent by 'Edit49'
		case ID_EDIT_50: // Notifications sent by 'Edit50'
		case ID_EDIT_51: // Notifications sent by 'Edit51'
		case ID_EDIT_52: // Notifications sent by 'Edit52'
		case ID_EDIT_53: // Notifications sent by 'Edit53'
		case ID_EDIT_54: // Notifications sent by 'Edit54'
		case ID_EDIT_55: // Notifications sent by 'Edit55'
		case ID_EDIT_56: // Notifications sent by 'Edit56'
		case ID_EDIT_57: // Notifications sent by 'Edit57'
		case ID_EDIT_58: // Notifications sent by 'Edit58'
		case ID_EDIT_59: // Notifications sent by 'Edit59'
		case ID_EDIT_60: // Notifications sent by 'Edit60'
		case ID_EDIT_61: // Notifications sent by 'Edit61'
		case ID_EDIT_62: // Notifications sent by 'Edit62'	
		case ID_EDIT_63: // Notifications sent by 'Edit63'
		case ID_EDIT_64: // Notifications sent by 'Edit64'
		case ID_EDIT_65: // Notifications sent by 'Edit65'
		case ID_EDIT_66: // Notifications sent by 'Edit66'
		case ID_EDIT_67: // Notifications sent by 'Edit67'
		case ID_EDIT_68: // Notifications sent by 'Edit68'
		case ID_EDIT_69: // Notifications sent by 'Edit69'
		case ID_EDIT_70: // Notifications sent by 'Edit70'
		case ID_EDIT_71: // Notifications sent by 'Edit71'
		case ID_EDIT_72: // Notifications sent by 'Edit72'	
		case ID_EDIT_73: // Notifications sent by 'Edit73'
		case ID_EDIT_74: // Notifications sent by 'Edit74'
		case ID_EDIT_75: // Notifications sent by 'Edit75'
		case ID_EDIT_76: // Notifications sent by 'Edit76'
		case ID_EDIT_77: // Notifications sent by 'Edit77'
		case ID_EDIT_78: // Notifications sent by 'Edit78'
		case ID_EDIT_79: // Notifications sent by 'Edit79'
		case ID_EDIT_80: // Notifications sent by 'Edit80'
		case ID_EDIT_81: // Notifications sent by 'Edit81'
		case ID_EDIT_82: // Notifications sent by 'Edit82'	
		case ID_EDIT_83: // Notifications sent by 'Edit83'
		case ID_EDIT_84: // Notifications sent by 'Edit84'
		case ID_EDIT_85: // Notifications sent by 'Edit85'
		case ID_EDIT_86: // Notifications sent by 'Edit86'
		case ID_EDIT_87: // Notifications sent by 'Edit87'
		case ID_EDIT_88: // Notifications sent by 'Edit88'
		case ID_EDIT_89: // Notifications sent by 'Edit89'
		case ID_EDIT_90: // Notifications sent by 'Edit90'
		case ID_EDIT_91: // Notifications sent by 'Edit91'
		case ID_EDIT_92: // Notifications sent by 'Edit92'	
		case ID_EDIT_93: // Notifications sent by 'Edit93'
		case ID_EDIT_94: // Notifications sent by 'Edit94'
		case ID_EDIT_95: // Notifications sent by 'Edit95'
		case ID_EDIT_96: // Notifications sent by 'Edit96'
		case ID_EDIT_97: // Notifications sent by 'Edit97'
		case ID_EDIT_98: // Notifications sent by 'Edit98'
		case ID_EDIT_99: // Notifications sent by 'Edit99'
		case ID_EDIT_100: // Notifications sent by 'Edit100'
    case ID_EDIT_101: // Notifications sent by 'Edit102'
    case ID_EDIT_102: // Notifications sent by 'Edit103'
    case ID_EDIT_103: // Notifications sent by 'Edit104'
    case ID_EDIT_104: // Notifications sent by 'Edit105'
    case ID_EDIT_105: // Notifications sent by 'Edit106'
    case ID_EDIT_106: // Notifications sent by 'Edit107'
    case ID_EDIT_107: // Notifications sent by 'Edit108'
		case ID_EDIT_108: // Notifications sent by 'Edit109'
		case ID_EDIT_109: // Notifications sent by 'Edit110'
		case ID_EDIT_110: // Notifications sent by 'Edit111'
		case ID_EDIT_111: // Notifications sent by 'Edit112'
		case ID_EDIT_112: // Notifications sent by 'Edit113'
		case ID_EDIT_113: // Notifications sent by 'Edit114'
		case ID_EDIT_114: // Notifications sent by 'Edit115'
		case ID_EDIT_115: // Notifications sent by 'Edit116'
		case ID_EDIT_116: // Notifications sent by 'Edit117'
		case ID_EDIT_117: // Notifications sent by 'Edit118'
		case ID_EDIT_118: // Notifications sent by 'Edit119'
		case ID_EDIT_119: // Notifications sent by 'Edit120'
		case ID_EDIT_120: // Notifications sent by 'Edit121'
		case ID_EDIT_121: // Notifications sent by 'Edit122'
		case ID_EDIT_122: // Notifications sent by 'Edit123'
		case ID_EDIT_123: // Notifications sent by 'Edit124'
		case ID_EDIT_124: // Notifications sent by 'Edit125'
		case ID_EDIT_125: // Notifications sent by 'Edit126'
		case ID_EDIT_126: // Notifications sent by 'Edit127'
		case ID_EDIT_127: // Notifications sent by 'Edit128'
		case ID_EDIT_128: // Notifications sent by 'Edit129'
		case ID_EDIT_129: // Notifications sent by 'Edit130'
		case ID_EDIT_130: // Notifications sent by 'Edit131'
		case ID_EDIT_131: // Notifications sent by 'Edit132'
		case ID_EDIT_132: // Notifications sent by 'Edit133'
		case ID_EDIT_133: // Notifications sent by 'Edit134'
		case ID_EDIT_134: // Notifications sent by 'Edit135'
		case ID_EDIT_135: // Notifications sent by 'Edit136'
		case ID_EDIT_136: // Notifications sent by 'Edit137'
		case ID_EDIT_137: // Notifications sent by 'Edit138'
		case ID_EDIT_138: // Notifications sent by 'Edit139'
		case ID_EDIT_139: // Notifications sent by 'Edit140'
		case ID_EDIT_140: // Notifications sent by 'Edit141'
		case ID_EDIT_141: // Notifications sent by 'Edit142'
		case ID_EDIT_142: // Notifications sent by 'Edit143'
		case ID_EDIT_143: // Notifications sent by 'Edit144'
		case ID_EDIT_144: // Notifications sent by 'Edit145'
		case ID_EDIT_145: // Notifications sent by 'Edit146'
		case ID_EDIT_146: // Notifications sent by 'Edit147'
		case ID_EDIT_147: // Notifications sent by 'Edit148'
		case ID_EDIT_148: // Notifications sent by 'Edit149'
		case ID_EDIT_149: // Notifications sent by 'Edit150'
		case ID_EDIT_150: // Notifications sent by 'Edit151'
		case ID_EDIT_151: // Notifications sent by 'Edit152'
		case ID_EDIT_152: // Notifications sent by 'Edit153'
		case ID_EDIT_153: // Notifications sent by 'Edit154'
		case ID_EDIT_154: // Notifications sent by 'Edit155'
		case ID_EDIT_155: // Notifications sent by 'Edit156'
		case ID_EDIT_156: // Notifications sent by 'Edit157'
		case ID_EDIT_157: // Notifications sent by 'Edit158'
		case ID_EDIT_158: // Notifications sent by 'Edit159'
		case ID_EDIT_159: // Notifications sent by 'Edit160'
		case ID_EDIT_160: // Notifications sent by 'Edit161'
		case ID_EDIT_161: // Notifications sent by 'Edit162'	
		case ID_EDIT_162: // Notifications sent by 'Edit163'
		case ID_EDIT_163: // Notifications sent by 'Edit164'
		case ID_EDIT_164: // Notifications sent by 'Edit165'
		case ID_EDIT_165: // Notifications sent by 'Edit166'
		case ID_EDIT_166: // Notifications sent by 'Edit167'
		case ID_EDIT_167: // Notifications sent by 'Edit168'
		case ID_EDIT_168: // Notifications sent by 'Edit169'
		case ID_EDIT_169: // Notifications sent by 'Edit170'
		case ID_EDIT_170: // Notifications sent by 'Edit171'
		case ID_EDIT_171: // Notifications sent by 'Edit172'	
		case ID_EDIT_172: // Notifications sent by 'Edit173'
		case ID_EDIT_173: // Notifications sent by 'Edit174'
		case ID_EDIT_174: // Notifications sent by 'Edit175'
		case ID_EDIT_175: // Notifications sent by 'Edit176'
		case ID_EDIT_176: // Notifications sent by 'Edit177'
		case ID_EDIT_177: // Notifications sent by 'Edit178'
		case ID_EDIT_178: // Notifications sent by 'Edit179'
		case ID_EDIT_179: // Notifications sent by 'Edit180'
		case ID_EDIT_180: // Notifications sent by 'Edit181'
		case ID_EDIT_181: // Notifications sent by 'Edit182'	
		case ID_EDIT_182: // Notifications sent by 'Edit183'
		case ID_EDIT_183: // Notifications sent by 'Edit184'
		case ID_EDIT_184: // Notifications sent by 'Edit185'
		case ID_EDIT_185: // Notifications sent by 'Edit186'
		case ID_EDIT_186: // Notifications sent by 'Edit187'
		case ID_EDIT_187: // Notifications sent by 'Edit188'
		case ID_EDIT_188: // Notifications sent by 'Edit189'
		case ID_EDIT_189: // Notifications sent by 'Edit190'
		case ID_EDIT_190: // Notifications sent by 'Edit191'
		case ID_EDIT_191: // Notifications sent by 'Edit192'	
		case ID_EDIT_192: // Notifications sent by 'Edit193'
		case ID_EDIT_193: // Notifications sent by 'Edit194'
		case ID_EDIT_194: // Notifications sent by 'Edit195'
		case ID_EDIT_195: // Notifications sent by 'Edit196'
		case ID_EDIT_196: // Notifications sent by 'Edit197'
		case ID_EDIT_197: // Notifications sent by 'Edit197'

      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						
			
						Edit_Index = Id - ID_EDIT_0;//计算当前EDIT的索引
			
						hDlg = CreateSelectDialog(pMsg->hWin,State.x,State.y);
						WM_MakeModal(hDlg);
						GUI_ExecCreatedDialog(hDlg);

						if(_acText[0] != 0)
						{		
								EDIT_SetFont(pMsg->hWinSrc, &GUI_FontSongTi12);
								EDIT_SetText(pMsg->hWinSrc,_acText);
								memset(_acText,0,sizeof(_acText));
								EDIT_SetBkColor(pMsg->hWinSrc,EDIT_CI_ENABLED,GUI_CYAN);
						}
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        break;
      }
      break;
    case ID_BUTTON_BACK: // Notifications sent by 'BACK'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						Clear_List(Ins_List_Head);         //清空链表所有结点
						GUI_EndDialog(hWin1_1,0);
						
        break;
      }
      break;
    case ID_BUTTON_RUN: // Notifications sent by 'RUN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
						if(Ins_List_Head->next)
						{
							_MessageBox(StringHZ[3],StringHZ[4], &Mb_Val);//先让用户确认
							if( Mb_Val == GUI_ID_OK) 
							{
								WriteFileProcess(program_name);//将链表内容写进Flash
								OS_SemPost(&RUN_SEM, OS_OPT_POST_1, 0, &err);//发送运行信号量
								Create_RunningWindow();
							}
							else
							{
								
							}
						}
						else
							_MessageBox(StringHZ[5],StringHZ[6], &Mb_Val);
						
        break;
      }
      break;

		case ID_SCROLLBAR_0: // Notifications sent by 'Scrollbar'
				 WM_GetScrollState(pMsg->hWinSrc, &ScrollState);
				 if (NCode == WM_NOTIFICATION_RELEASED)
				{
				//the envent that trigger by kick the scrollbar
						
						if (_yOld != ScrollState.v) {
							int y;
							
							for( y = 0; y < MAX_EDIT_NUM; y++)
							{
								if(hEdit[y] != NULL && hText[y] != NULL)
								{ //the step is 30 pixels
									WM_MoveWindow(hEdit[y] , 0, (_yOld - ScrollState.v)*30);
									WM_MoveWindow(hText[y] , 0, (_yOld - ScrollState.v)*30);
								}
								WM_GetWindowRectEx(hEdit[y], &rect);//如果EDIT控件所在位置越界，则隐藏该EDIT
								if(rect.y1 >= 300 || rect.y1 <= 20)
								{
									WM_HideWindow(hEdit[y]);
									WM_HideWindow(hText[y]);
								}
								else
								{
									WM_ShowWindow(hEdit[y]);
									WM_ShowWindow(hText[y]);
								}
							}
						
						}
						_yOld = ScrollState.v;
      }
      break;
    }
		break;
		case  WM_PAINT:
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow_1_1(void);
WM_HWIN CreateWindow_1_1(void) {
	
  hWin1_1 = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin1_1;
}


/***************************** 阿波罗科技 www.apollorobot.cn (END OF FILE) *********************************/
