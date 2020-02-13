/*file generated with acs_test_data_generator.m octave/matlab script 13-Feb-2020 13:14:38*/
#include <stdint.h>

/* primary APPE decoding look up table */
const uint32_t acsPrimDecodingLUT[1025] =
{
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    1,    1,    2,    2,    2,    3, 
   3,    3,    3,    4,    4,    4,    5,    5,    5,    6,    6,    6,    7,    7,    7,    8, 
   8,    8,    8,    9,    9,    9,   10,   10,   10,   11,   11,   11,   12,   12,   12,   13, 
  13,   13,   14,   14,   14,   15,   15,   15,   16,   16,   16,   17,   17,   17,   18,   18, 
  18,   19,   19,   19,   20,   20,   21,   21,   21,   22,   22,   22,   23,   23,   23,   24, 
  24,   24,   25,   25,   26,   26,   26,   27,   27,   27,   28,   28,   29,   29,   29,   30, 
  30,   30,   31,   31,   32,   32,   32,   33,   33,   34,   34,   34,   35,   35,   36,   36, 
  36,   37,   37,   38,   38,   38,   39,   39,   40,   40,   40,   41,   41,   42,   42,   42, 
  43,   43,   44,   44,   45,   45,   45,   46,   46,   47,   47,   48,   48,   48,   49,   49, 
  50,   50,   51,   51,   51,   52,   52,   53,   53,   54,   54,   55,   55,   56,   56,   56, 
  57,   57,   58,   58,   59,   59,   60,   60,   61,   61,   62,   62,   62,   63,   63,   64, 
  64,   65,   65,   66,   66,   67,   67,   68,   68,   69,   69,   70,   70,   71,   71,   72, 
  72,   73,   73,   74,   74,   75,   75,   76,   76,   77,   77,   78,   78,   79,   80,   80, 
  81,   81,   82,   82,   83,   83,   84,   84,   85,   85,   86,   86,   87,   88,   88,   89, 
  89,   90,   90,   91,   91,   92,   93,   93,   94,   94,   95,   95,   96,   97,   97,   98, 
  98,   99,   99,  100,  101,  101,  102,  102,  103,  104,  104,  105,  105,  106,  107,  107, 
 108,  108,  109,  110,  110,  111,  112,  112,  113,  113,  114,  115,  115,  116,  117,  117, 
 118,  118,  119,  120,  120,  121,  122,  122,  123,  124,  124,  125,  126,  126,  127,  128, 
 128,  129,  130,  130,  131,  132,  132,  133,  134,  134,  135,  136,  136,  137,  138,  139, 
 139,  140,  141,  141,  142,  143,  143,  144,  145,  146,  146,  147,  148,  149,  149,  150, 
 151,  151,  152,  153,  154,  154,  155,  156,  157,  157,  158,  159,  160,  160,  161,  162, 
 163,  164,  164,  165,  166,  167,  167,  168,  169,  170,  171,  171,  172,  173,  174,  175, 
 175,  176,  177,  178,  179,  179,  180,  181,  182,  183,  184,  184,  185,  186,  187,  188, 
 189,  189,  190,  191,  192,  193,  194,  195,  195,  196,  197,  198,  199,  200,  201,  202, 
 202,  203,  204,  205,  206,  207,  208,  209,  210,  211,  211,  212,  213,  214,  215,  216, 
 217,  218,  219,  220,  221,  222,  223,  224,  224,  225,  226,  227,  228,  229,  230,  231, 
 232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,  247, 
 248,  249,  250,  251,  252,  253,  254,  255,  256,  257,  258,  259,  261,  262,  263,  264, 
 265,  266,  267,  268,  269,  270,  271,  272,  273,  275,  276,  277,  278,  279,  280,  281, 
 282,  283,  285,  286,  287,  288,  289,  290,  291,  293,  294,  295,  296,  297,  298,  299, 
 301,  302,  303,  304,  305,  307,  308,  309,  310,  311,  313,  314,  315,  316,  317,  319, 
 320,  321,  322,  324,  325,  326,  327,  329,  330,  331,  332,  334,  335,  336,  337,  339, 
 340,  341,  343,  344,  345,  347,  348,  349,  350,  352,  353,  354,  356,  357,  358,  360, 
 361,  363,  364,  365,  367,  368,  369,  371,  372,  373,  375,  376,  378,  379,  380,  382, 
 383,  385,  386,  388,  389,  390,  392,  393,  395,  396,  398,  399,  401,  402,  404,  405, 
 406,  408,  409,  411,  412,  414,  415,  417,  418,  420,  422,  423,  425,  426,  428,  429, 
 431,  432,  434,  435,  437,  439,  440,  442,  443,  445,  447,  448,  450,  451,  453,  455, 
 456,  458,  460,  461,  463,  464,  466,  468,  469,  471,  473,  474,  476,  478,  480,  481, 
 483,  485,  486,  488,  490,  492,  493,  495,  497,  499,  500,  502,  504,  506,  507,  509, 
 511,  513,  515,  516,  518,  520,  522,  524,  525,  527,  529,  531,  533,  535,  537,  538, 
 540,  542,  544,  546,  548,  550,  552,  554,  555,  557,  559,  561,  563,  565,  567,  569, 
 571,  573,  575,  577,  579,  581,  583,  585,  587,  589,  591,  593,  595,  597,  599,  601, 
 603,  605,  607,  609,  612,  614,  616,  618,  620,  622,  624,  626,  628,  631,  633,  635, 
 637,  639,  641,  643,  646,  648,  650,  652,  654,  657,  659,  661,  663,  666,  668,  670, 
 672,  675,  677,  679,  681,  684,  686,  688,  691,  693,  695,  698,  700,  702,  705,  707, 
 709,  712,  714,  717,  719,  721,  724,  726,  729,  731,  733,  736,  738,  741,  743,  746, 
 748,  751,  753,  756,  758,  761,  763,  766,  768,  771,  773,  776,  779,  781,  784,  786, 
 789,  792,  794,  797,  799,  802,  805,  807,  810,  813,  815,  818,  821,  823,  826,  829, 
 832,  834,  837,  840,  843,  845,  848,  851,  854,  857,  859,  862,  865,  868,  871,  873, 
 876,  879,  882,  885,  888,  891,  894,  897,  899,  902,  905,  908,  911,  914,  917,  920, 
 923,  926,  929,  932,  935,  938,  941,  944,  947,  951,  954,  957,  960,  963,  966,  969, 
 972,  975,  979,  982,  985,  988,  991,  995, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
0x8C07870E};

/* backup APPE decoding look up table */
const uint32_t acsBackDecodingLUT[1025] =
{
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 
   0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    1,    1,    2,    2,    2,    3, 
   3,    3,    3,    4,    4,    4,    5,    5,    5,    6,    6,    6,    7,    7,    7,    8, 
   8,    8,    8,    9,    9,    9,   10,   10,   10,   11,   11,   11,   12,   12,   12,   13, 
  13,   13,   14,   14,   14,   15,   15,   15,   16,   16,   16,   17,   17,   17,   18,   18, 
  18,   19,   19,   19,   20,   20,   21,   21,   21,   22,   22,   22,   23,   23,   23,   24, 
  24,   24,   25,   25,   26,   26,   26,   27,   27,   27,   28,   28,   29,   29,   29,   30, 
  30,   30,   31,   31,   32,   32,   32,   33,   33,   34,   34,   34,   35,   35,   36,   36, 
  36,   37,   37,   38,   38,   38,   39,   39,   40,   40,   40,   41,   41,   42,   42,   42, 
  43,   43,   44,   44,   45,   45,   45,   46,   46,   47,   47,   48,   48,   48,   49,   49, 
  50,   50,   51,   51,   51,   52,   52,   53,   53,   54,   54,   55,   55,   56,   56,   56, 
  57,   57,   58,   58,   59,   59,   60,   60,   61,   61,   62,   62,   62,   63,   63,   64, 
  64,   65,   65,   66,   66,   67,   67,   68,   68,   69,   69,   70,   70,   71,   71,   72, 
  72,   73,   73,   74,   74,   75,   75,   76,   76,   77,   77,   78,   78,   79,   80,   80, 
  81,   81,   82,   82,   83,   83,   84,   84,   85,   85,   86,   86,   87,   88,   88,   89, 
  89,   90,   90,   91,   91,   92,   93,   93,   94,   94,   95,   95,   96,   97,   97,   98, 
  98,   99,   99,  100,  101,  101,  102,  102,  103,  104,  104,  105,  105,  106,  107,  107, 
 108,  108,  109,  110,  110,  111,  112,  112,  113,  113,  114,  115,  115,  116,  117,  117, 
 118,  118,  119,  120,  120,  121,  122,  122,  123,  124,  124,  125,  126,  126,  127,  128, 
 128,  129,  130,  130,  131,  132,  132,  133,  134,  134,  135,  136,  136,  137,  138,  139, 
 139,  140,  141,  141,  142,  143,  143,  144,  145,  146,  146,  147,  148,  149,  149,  150, 
 151,  151,  152,  153,  154,  154,  155,  156,  157,  157,  158,  159,  160,  160,  161,  162, 
 163,  164,  164,  165,  166,  167,  167,  168,  169,  170,  171,  171,  172,  173,  174,  175, 
 175,  176,  177,  178,  179,  179,  180,  181,  182,  183,  184,  184,  185,  186,  187,  188, 
 189,  189,  190,  191,  192,  193,  194,  195,  195,  196,  197,  198,  199,  200,  201,  202, 
 202,  203,  204,  205,  206,  207,  208,  209,  210,  211,  211,  212,  213,  214,  215,  216, 
 217,  218,  219,  220,  221,  222,  223,  224,  224,  225,  226,  227,  228,  229,  230,  231, 
 232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,  247, 
 248,  249,  250,  251,  252,  253,  254,  255,  256,  257,  258,  259,  261,  262,  263,  264, 
 265,  266,  267,  268,  269,  270,  271,  272,  273,  275,  276,  277,  278,  279,  280,  281, 
 282,  283,  285,  286,  287,  288,  289,  290,  291,  293,  294,  295,  296,  297,  298,  299, 
 301,  302,  303,  304,  305,  307,  308,  309,  310,  311,  313,  314,  315,  316,  317,  319, 
 320,  321,  322,  324,  325,  326,  327,  329,  330,  331,  332,  334,  335,  336,  337,  339, 
 340,  341,  343,  344,  345,  347,  348,  349,  350,  352,  353,  354,  356,  357,  358,  360, 
 361,  363,  364,  365,  367,  368,  369,  371,  372,  373,  375,  376,  378,  379,  380,  382, 
 383,  385,  386,  388,  389,  390,  392,  393,  395,  396,  398,  399,  401,  402,  404,  405, 
 406,  408,  409,  411,  412,  414,  415,  417,  418,  420,  422,  423,  425,  426,  428,  429, 
 431,  432,  434,  435,  437,  439,  440,  442,  443,  445,  447,  448,  450,  451,  453,  455, 
 456,  458,  460,  461,  463,  464,  466,  468,  469,  471,  473,  474,  476,  478,  480,  481, 
 483,  485,  486,  488,  490,  492,  493,  495,  497,  499,  500,  502,  504,  506,  507,  509, 
 511,  513,  515,  516,  518,  520,  522,  524,  525,  527,  529,  531,  533,  535,  537,  538, 
 540,  542,  544,  546,  548,  550,  552,  554,  555,  557,  559,  561,  563,  565,  567,  569, 
 571,  573,  575,  577,  579,  581,  583,  585,  587,  589,  591,  593,  595,  597,  599,  601, 
 603,  605,  607,  609,  612,  614,  616,  618,  620,  622,  624,  626,  628,  631,  633,  635, 
 637,  639,  641,  643,  646,  648,  650,  652,  654,  657,  659,  661,  663,  666,  668,  670, 
 672,  675,  677,  679,  681,  684,  686,  688,  691,  693,  695,  698,  700,  702,  705,  707, 
 709,  712,  714,  717,  719,  721,  724,  726,  729,  731,  733,  736,  738,  741,  743,  746, 
 748,  751,  753,  756,  758,  761,  763,  766,  768,  771,  773,  776,  779,  781,  784,  786, 
 789,  792,  794,  797,  799,  802,  805,  807,  810,  813,  815,  818,  821,  823,  826,  829, 
 832,  834,  837,  840,  843,  845,  848,  851,  854,  857,  859,  862,  865,  868,  871,  873, 
 876,  879,  882,  885,  888,  891,  894,  897,  899,  902,  905,  908,  911,  914,  917,  920, 
 923,  926,  929,  932,  935,  938,  941,  944,  947,  951,  954,  957,  960,  963,  966,  969, 
 972,  975,  979,  982,  985,  988,  991,  995, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 
0x8C07870E};
