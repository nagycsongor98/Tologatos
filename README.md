# Tologatos
Implementált kapcsolók:
-input input.txt, ha nincs a standart bemenetöl olvas be.
-solseq, kiírja a lépéseket.
-pcost, optimális lépések száma (költség).
-nvisited, meglátogatott csomópontok száma.
-h 1, a rossz elemek száma heurisztikát használja.
-h 2  a manhattan heurisztikát használja.
Összehasonlítása:
  3 
  1  6  8
  5  0  2
  4  7  3
  -h 1:
    költség:26
    meglátogatott csomópontok számar:32539
  -h 2:
    költség:26
    meglátogatott csomópontok számar:2394
    
  8
   1     9     2     3     4     5     6     7
  16     8    10    11    13    14    22    15
  24    18    19    20    12    21    23    31
  26    17    34    27    28    29     0    30
  25    33    42    35    36    37    38    39
  32    49    41    43    44    45    46    47
  40    56    50    51    52    53    54    55
  57    48    58    59    60    61    62    6
  -h 1:
    költség:31
    meglátogatott csomópontok számar:236
  -h 2:
    költség:31
    meglátogatott csomópontok számar:32
