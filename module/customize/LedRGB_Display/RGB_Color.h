#ifndef __RGB_COLOR_H
#define __RGB_COLOR_H


#ifdef __cplusplus
	extern "C"{
#endif

// refer to: http://www.w3.org/TR/css3-color/
// refer to: https://en.wikipedia.org/wiki/X11_color_names

#define COLOR_RGB_AliceBlue                 0xF0F8FF
#define COLOR_RGB_AntiqueWhite              0xFAEBD7
#define COLOR_RGB_Aqua                      0x00FFFF
#define COLOR_RGB_Aquamarine                0x7FFFD4
#define COLOR_RGB_Azure                     0xF0FFFF
#define COLOR_RGB_Beige                     0xF5F5DC
#define COLOR_RGB_Bisque                    0xFFE4C4
#define COLOR_RGB_Black                     0x000000
#define COLOR_RGB_BlanchedAlmond            0xFFEBCD
#define COLOR_RGB_Blue                      0x0000FF
#define COLOR_RGB_BlueViolet                0x8A2BE2
#define COLOR_RGB_Brown                     0xA52A2A
#define COLOR_RGB_BurlyWood                 0xDEB887
#define COLOR_RGB_CadetBlue                 0x5F9EA0
#define COLOR_RGB_Chartreuse                0x7FFF00
#define COLOR_RGB_Chocolate                 0xD2691E
#define COLOR_RGB_Coral                     0xFF7F50
#define COLOR_RGB_CornflowerBlue            0x6495ED
#define COLOR_RGB_Cornsilk                  0xFFF8DC
#define COLOR_RGB_Crimson                   0xDC143C
#define COLOR_RGB_Cyan                      0x00FFFF
#define COLOR_RGB_DarkBlue                  0x00008B
#define COLOR_RGB_DarkCyan                  0x008B8B
#define COLOR_RGB_DarkGoldenRod             0xB8860B
#define COLOR_RGB_DarkGray                  0xA9A9A9
#define COLOR_RGB_DarkGreen                 0x006400
#define COLOR_RGB_DarkKhaki                 0xBDB76B
#define COLOR_RGB_DarkMagenta               0x8B008B
#define COLOR_RGB_DarkOliveGreen            0x556B2F
#define COLOR_RGB_DarkOrange                0xFF8C00
#define COLOR_RGB_DarkOrchid                0x9932CC
#define COLOR_RGB_DarkRed                   0x8B0000
#define COLOR_RGB_DarkSalmon                0xE9967A
#define COLOR_RGB_DarkSeaGreen              0x8FBC8F
#define COLOR_RGB_DarkSlateBlue             0x483D8B
#define COLOR_RGB_DarkSlateGray             0x2F4F4F
#define COLOR_RGB_DarkTurquoise             0x00CED1
#define COLOR_RGB_DarkViolet                0x9400D3
#define COLOR_RGB_DeepPink                  0xFF1493
#define COLOR_RGB_DeepSkyBlue               0x00BFFF
#define COLOR_RGB_DimGray                   0x696969
#define COLOR_RGB_DodgerBlue                0x1E90FF
#define COLOR_RGB_FireBrick                 0xB22222
#define COLOR_RGB_FloralWhite               0xFFFAF0
#define COLOR_RGB_ForestGreen               0x228B22
#define COLOR_RGB_Fuchsia                   0xFF00FF
#define COLOR_RGB_Gainsboro                 0xDCDCDC
#define COLOR_RGB_GhostWhite                0xF8F8FF
#define COLOR_RGB_Gold                      0xFFD700
#define COLOR_RGB_GoldenRod                 0xDAA520
#define COLOR_RGB_Gray                      0x808080
#define COLOR_RGB_Green                     0x008000
#define COLOR_RGB_GreenYellow               0xADFF2F
#define COLOR_RGB_HoneyDew                  0xF0FFF0
#define COLOR_RGB_HotPink                   0xFF69B4
#define COLOR_RGB_IndianRed                 0xCD5C5C
#define COLOR_RGB_Indigo                    0x4B0082
#define COLOR_RGB_Ivory                     0xFFFFF0
#define COLOR_RGB_Khaki                     0xF0E68C
#define COLOR_RGB_Lavender                  0xE6E6FA
#define COLOR_RGB_LavenderBlush             0xFFF0F5
#define COLOR_RGB_LawnGreen                 0x7CFC00
#define COLOR_RGB_LemonChiffon              0xFFFACD
#define COLOR_RGB_LightBlue                 0xADD8E6
#define COLOR_RGB_LightCoral                0xF08080
#define COLOR_RGB_LightCyan                 0xE0FFFF
#define COLOR_RGB_LightGoldenRodYellow      0xFAFAD2
#define COLOR_RGB_LightGray                 0xD3D3D3
#define COLOR_RGB_LightGreen                0x90EE90
#define COLOR_RGB_LightPink                 0xFFB6C1
#define COLOR_RGB_LightSalmon               0xFFA07A
#define COLOR_RGB_LightSeaGreen             0x20B2AA
#define COLOR_RGB_LightSkyBlue              0x87CEFA
#define COLOR_RGB_LightSlateGray            0x778899
#define COLOR_RGB_LightSteelBlue            0xB0C4DE
#define COLOR_RGB_LightYellow               0xFFFFE0
#define COLOR_RGB_Lime                      0x00FF00
#define COLOR_RGB_LimeGreen                 0x32CD32
#define COLOR_RGB_Linen                     0xFAF0E6
#define COLOR_RGB_Magenta                   0xFF00FF
#define COLOR_RGB_Maroon                    0x800000
#define COLOR_RGB_MediumAquaMarine          0x66CDAA
#define COLOR_RGB_MediumBlue                0x0000CD
#define COLOR_RGB_MediumOrchid              0xBA55D3
#define COLOR_RGB_MediumPurple              0x9370DB
#define COLOR_RGB_MediumSeaGreen            0x3CB371
#define COLOR_RGB_MediumSlateBlue           0x7B68EE
#define COLOR_RGB_MediumSpringGreen         0x00FA9A
#define COLOR_RGB_MediumTurquoise           0x48D1CC
#define COLOR_RGB_MediumVioletRed           0xC71585
#define COLOR_RGB_MidnightBlue              0x191970
#define COLOR_RGB_MintCream                 0xF5FFFA
#define COLOR_RGB_MistyRose                 0xFFE4E1
#define COLOR_RGB_Moccasin                  0xFFE4B5
#define COLOR_RGB_NavajoWhite               0xFFDEAD
#define COLOR_RGB_Navy                      0x000080
#define COLOR_RGB_OldLace                   0xFDF5E6
#define COLOR_RGB_Olive                     0x808000
#define COLOR_RGB_OliveDrab                 0x6B8E23
#define COLOR_RGB_Orange                    0xFFA500
#define COLOR_RGB_OrangeRed                 0xFF4500
#define COLOR_RGB_Orchid                    0xDA70D6
#define COLOR_RGB_PaleGoldenRod             0xEEE8AA
#define COLOR_RGB_PaleGreen                 0x98FB98
#define COLOR_RGB_PaleTurquoise             0xAFEEEE
#define COLOR_RGB_PaleVioletRed             0xDB7093
#define COLOR_RGB_PapayaWhip                0xFFEFD5
#define COLOR_RGB_PeachPuff                 0xFFDAB9
#define COLOR_RGB_Peru                      0xCD853F
#define COLOR_RGB_Pink                      0xFFC0CB
#define COLOR_RGB_Plum                      0xDDA0DD
#define COLOR_RGB_PowderBlue                0xB0E0E6
#define COLOR_RGB_Purple                    0x800080
#define COLOR_RGB_RebeccaPurple             0x663399
#define COLOR_RGB_Red                       0xFF0000
#define COLOR_RGB_RosyBrown                 0xBC8F8F
#define COLOR_RGB_RoyalBlue                 0x4169E1
#define COLOR_RGB_SaddleBrown               0x8B4513
#define COLOR_RGB_Salmon                    0xFA8072
#define COLOR_RGB_SandyBrown                0xF4A460
#define COLOR_RGB_SeaGreen                  0x2E8B57
#define COLOR_RGB_SeaShell                  0xFFF5EE
#define COLOR_RGB_Sienna                    0xA0522D
#define COLOR_RGB_Silver                    0xC0C0C0
#define COLOR_RGB_SkyBlue                   0x87CEEB
#define COLOR_RGB_SlateBlue                 0x6A5ACD
#define COLOR_RGB_SlateGray                 0x708090
#define COLOR_RGB_Snow                      0xFFFAFA
#define COLOR_RGB_SpringGreen               0x00FF7F
#define COLOR_RGB_SteelBlue                 0x4682B4
#define COLOR_RGB_Tan                       0xD2B48C
#define COLOR_RGB_Teal                      0x008080
#define COLOR_RGB_Thistle                   0xD8BFD8
#define COLOR_RGB_Tomato                    0xFF6347
#define COLOR_RGB_Turquoise                 0x40E0D0
#define COLOR_RGB_Violet                    0xEE82EE
#define COLOR_RGB_Wheat                     0xF5DEB3
#define COLOR_RGB_White                     0xFFFFFF
#define COLOR_RGB_WhiteSmoke                0xF5F5F5
#define COLOR_RGB_Yellow                    0xFFFF00
#define COLOR_RGB_YellowGreen               0x9ACD32

#define COLOR_WITH_FULL_TRANSPORT(color)           (uint32_t)(0xff000000|color)

#ifdef __cplusplus
		}
#endif


#endif
